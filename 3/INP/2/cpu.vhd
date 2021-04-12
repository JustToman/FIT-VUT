-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti 
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 --Program counter signals
 signal PC_INC : std_logic;
 signal PC_DEC : std_logic;
 signal PC_LD  : std_logic;
 signal PC_REG : std_logic_vector(11 downto 0);
 
 --RAS SIGNALS
 signal RAS_PUSH : std_logic;
 signal RAS_POP  : std_logic;
 signal RAS_STACK : std_logic_vector(191 downto 0); 
 
 --CNT SIGNALS
 signal CNT_INC : std_logic;
 signal CNT_DEC : std_logic;
 signal CNT_REG : std_logic_vector(3 downto 0);
 
 --PTR signals
 signal PTR_INC : std_logic;
 signal PTR_DEC : std_logic;
 signal PTR_REG : std_logic_vector (9 downto 0);
 
 --MUX signals
 signal MUX_SELECT: std_logic_vector(1 downto 0);
 signal MUX_OUT:    std_logic_vector(7 downto 0);
 
 --FSM STATES
 type state is (s_start, s_fetch, s_decode, s_inc_ptr, s_dec_ptr, s_print, s_input, s_while_end, s_while_start, s_while_end_cmp,
					 s_dec_val, s_inc_val, s_halt,s_dec_val_write,s_inc_val_write, s_input_waiting, s_while_in, s_while_fetch_op, s_while_throw_away,
					 s_inc_val_sel_mux, s_dec_val_sel_mux);
 signal FSM_CUR_STATE,FSM_NEXT_STATE: state;


begin

---------------- PC REGISTER ----------------------
	pc: process(CLK,RESET, PC_INC, PC_DEC, PC_LD, PC_REG) begin
		if RESET = '1' then
		  PC_REG <= (others => '0');
		elsif rising_edge(CLK) then
			if PC_INC = '1' then
				PC_REG <= PC_REG + 1;
			elsif PC_DEC = '1' then
				PC_REG <= PC_REG - 1;
			elsif PC_LD = '1' then
				PC_REG <= RAS_STACK(11 downto 0);
			end if;
		end if;
		CODE_ADDR <= PC_REG;
	end process;
	
---------------- RAS REGISTER ----------------------
	ras: process(CLK,RESET, RAS_PUSH, RAS_POP) 
	begin
	if RESET = '1' then
			RAS_STACK <= (others => '0');
	elsif rising_edge(CLK) then
		if RAS_PUSH = '1' then
			for n in 0 to 14 loop
				RAS_STACK(((n+1)*12+11) downto ((n+1)*12)) <= RAS_STACK(((n*12)+11) downto (n * 12));
			end loop;
			RAS_STACK(11 downto 0) <= PC_REG;
		elsif RAS_POP = '1' then
			for n in 15 downto 1 loop
				RAS_STACK(((n-1)*12+11) downto ((n-1)*12)) <= RAS_STACK(((n*12)+11) downto (n * 12));
			end loop;
		end if;
	end if;
	end process;
	
---------------- CNT REGISTER ----------------------
	cnt: process(CLK,RESET, CNT_INC, CNT_DEC)
	begin
	if RESET = '1' then
	CNT_REG <= (others => '0');
	elsif rising_edge(CLK) then
		if CNT_INC = '1' then
			CNT_REG <= CNT_REG + 1;
		elsif CNT_DEC = '1' then
			CNT_REG <= CNT_REG - 1;
		end if;
	end if;
	end process;


---------------- PTR REGISTER ----------------------
	ptr: process(CLK,RESET,PTR_INC,PTR_DEC, PTR_REG)
	begin
	if RESET = '1' then
		PTR_REG <= (others => '0');
   elsif	rising_edge(CLK) then
		if PTR_INC = '1' then
			if PTR_REG = "1111111111" then
				PTR_REG <= "0000000000";
			else
				PTR_REG <= PTR_REG + 1;
			end if;
		elsif PTR_DEC = '1' then
			if PTR_REG = "0000000000" then
				PTR_REG <= "1111111111";
			else
				PTR_REG <= PTR_REG -1;
			end if;
		end if;
	end if;
	DATA_ADDR <= PTR_REG;
	end process;
	
-----------------MUX-----------------------
	mux: process(CLK,RESET,IN_DATA,MUX_SELECT,DATA_RDATA)
	begin
		if rising_edge(CLK) then
			case MUX_SELECT is
			when "00" =>
				MUX_OUT <= IN_DATA;
			when "01" =>
				MUX_OUT <= DATA_RDATA - 1;
			when "11" =>
				MUX_OUT <= DATA_RDATA + 1;
			when others =>
				MUX_OUT <= DATA_RDATA;
			end case;
		end if;
	end process;
	
---------------- FSM ----------------------
	fsm_state_changer: process(CLK,RESET,FSM_NEXT_STATE, EN)
	begin
		if RESET = '1' then
			FSM_CUR_STATE <= s_start;
		elsif rising_edge(CLK) and EN = '1' then
			FSM_CUR_STATE <= FSM_NEXT_STATE;
		end if;
	end process;
	
	fsm_nstate: process(FSM_CUR_STATE, OUT_BUSY, MUX_OUT, IN_VLD, CODE_DATA,DATA_RDATA,CNT_REG)
	variable inst : std_logic_vector(7 downto 0);
	
	begin
	
	PTR_INC <= '0';
	PTR_DEC <= '0';
	
	PC_INC <= '0';
	PC_DEC <= '0';
	PC_LD <= '0';
	
	RAS_PUSH <= '0';
	RAS_POP <= '0';
	
	CNT_INC <= '0';
	CNT_DEC <= '0';
	
	CODE_EN <= '0';
	DATA_EN <= '0';
	
	DATA_WE <= '0';
	OUT_WE <= '0';
	
	DATA_WDATA  <= DATA_RDATA;
	OUT_DATA <= DATA_RDATA;
	
	MUX_SELECT <= "10";
	
	case FSM_CUR_STATE is
	----- State START -----
	when s_start =>
		fsm_next_state <= s_fetch;
	when s_fetch =>
		CODE_EN <= '1';
		fsm_next_state <= s_decode;
	when s_decode =>
		if CODE_DATA = 62 then
			fsm_next_state <= s_inc_ptr;
		elsif CODE_DATA= 60 then
			fsm_next_state <= s_dec_ptr;
		elsif CODE_DATA= 43 then
			fsm_next_state <= s_inc_val;
		elsif CODE_DATA = 45 then
			fsm_next_state <= s_dec_val;
		elsif CODE_DATA = 91 then
			fsm_next_state <= s_while_start;
		elsif CODE_DATA = 93 then
			fsm_next_state <= s_while_end;
		elsif CODE_DATA = 46 then
			fsm_next_state <= s_print;
		elsif CODE_DATA = 44 then
			fsm_next_state <= s_input;
		elsif CODE_DATA = 0 then
			fsm_next_state <= s_halt;
		else
			PC_INC <= '1';
			fsm_next_state <= s_fetch;
		end if;
		
	when s_inc_ptr =>
		PTR_INC <= '1';
		PC_INC <= '1';
	   fsm_next_state <= s_fetch;
	when s_dec_ptr =>
		PTR_DEC <= '1';
		PC_INC <= '1';
		fsm_next_state <= s_fetch;
	when s_inc_val =>
		-------Allowing reading--------
		DATA_EN <= '1';
		DATA_WE <= '0';
		fsm_next_state <= s_inc_val_sel_mux;
	when s_inc_val_sel_mux =>
	   ------Allowing writing and writing------
		MUX_SELECT <= "11";
		fsm_next_state <= s_inc_val_write;
	when s_inc_val_write =>
		DATA_WDATA <= MUX_OUT;
		DATA_EN <= '1';
		DATA_WE <= '1';
		PC_INC <= '1';
		fsm_next_state <= s_fetch;
	when s_dec_val =>
		DATA_EN <= '1';
		DATA_WE <= '0';
		fsm_next_state <= s_dec_val_sel_mux;
	when s_dec_val_sel_mux =>
		MUX_SELECT <= "01";
		fsm_next_state <= s_dec_val_write;
	when s_dec_val_write =>
		DATA_WDATA <= MUX_OUT;
		DATA_EN <= '1';
		DATA_WE <= '1';
		PC_INC <= '1';
		fsm_next_state <= s_fetch;
	when s_print =>
		if OUT_BUSY = '1' then 
			DATA_EN <= '1';
			DATA_WE <= '0';
			fsm_next_state <= s_print;
		else
			OUT_DATA <= DATA_RDATA;
			OUT_WE <= '1';
			PC_INC <= '1';
			fsm_next_state <= s_fetch;
		end if;
	when s_input =>
		IN_REQ <= '1';
		fsm_next_state <= s_input_waiting;
	when s_input_waiting =>
		if IN_VLD = '0' then
			fsm_next_state <= s_input_waiting;
		else
			MUX_SELECT <= "00";
			PC_INC <= '1';
			fsm_next_state <= s_fetch;
		end if;
		
	when s_while_start =>
		PC_INC <= '1';
		DATA_WE <= '0';
		DATA_EN <= '1';
		fsm_next_state <= s_while_in;
		
	when s_while_in =>
		if DATA_RDATA = 0 then
			CNT_INC <= '1';
			fsm_next_state <= s_while_fetch_op;
		else
			RAS_PUSH <= '1';
			fsm_next_state <= s_fetch;
		end if;
			
	when s_while_fetch_op =>
		if CNT_REG > 0 then
			CODE_EN <= '1';
			fsm_next_state <= s_while_throw_away;
		else
			fsm_next_state <= s_fetch;
		end if;
	when s_while_throw_away =>
		if CODE_DATA = 91 then
			CNT_INC <= '1';
		elsif CODE_DATA = 93 then
			CNT_DEC <= '1';
		
		end if;
		PC_INC <= '1';
		fsm_next_state <= s_while_fetch_op;
	when s_while_end =>
	   DATA_EN <= '1';
		DATA_WE <= '0';
		fsm_next_state <= s_while_end_cmp;
	when s_while_end_cmp =>
		if DATA_RDATA = 0 then
			PC_INC <= '1';
			RAS_POP <= '1';
		else
			PC_LD <= '1';
		end if;
		fsm_next_state <= s_fetch;
	when s_halt =>
		fsm_next_state <= s_halt;
	when others =>
		fsm_next_state <= s_start;
	end case;
		
	end process;


 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
 
end behavioral;
 
