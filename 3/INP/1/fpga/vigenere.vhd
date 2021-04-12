library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);
         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.

architecture behavioral of vigenere is

    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.
	 
	 type t_state is (minus, plus);
	 
	 signal SHIFT_BY: std_logic_vector(7 downto 0);
	 signal CORRECTED_PLUS: std_logic_vector(7 downto 0);
	 signal CORRECTED_MINUS: std_logic_vector(7 downto 0);
	 signal FSM_OUT: std_logic_vector(1 downto 0);
	 signal current_state, next_state: t_state;
	 
begin
	shift_counter : process(KEY,DATA) is
		begin
			SHIFT_BY <= KEY - 64; 
		end process;
	
	plus_adder: process(DATA,SHIFT_BY)
		variable tmp: std_logic_vector(7 downto 0);
		begin
				if (DATA >= 65) and (DATA <= 90) then
					tmp := DATA + SHIFT_BY;
					if (tmp > 90) then
						CORRECTED_PLUS <= tmp - 26;
					else
						CORRECTED_PLUS <= tmp;
					end if;
				else
					CORRECTED_PLUS <= "00100011";
				end if;
		end process;
	
	minus_adder: process(DATA,SHIFT_BY) is
		variable tmp: std_logic_vector(7 downto 0);
		begin
				if (DATA >= 65) and (DATA <= 90) then
					tmp := DATA - SHIFT_BY;
					if (tmp < 65) then
						CORRECTED_MINUS <= tmp + 26;
					else
						CORRECTED_MINUS <= tmp;
					end if;
				else
					CORRECTED_MINUS <= "00100011";
				end if;
		end process;
		
	 fsm_sync_logic: process(RST,CLK)
	 begin
		if RST = '1' then
			current_state <= plus;
		elsif rising_edge(CLK) then
			current_state <= next_state;		
		end if;
	 end process;

	fsm_state_changer: process(DATA,current_state) is
	begin
	case (current_state) is
		when plus =>
			next_state <= minus;
		when minus =>
			next_state <= plus;
	end case;
	end process;
	
	fsm_out_changer: process(current_state, RST) is
	begin
	case (current_state) is
		when plus =>
			if(RST = '1') then
				fsm_out <= "00";
			else
				fsm_out <= "01";
			end if;
		when minus => 
			fsm_out <= "10";
		end case;
	end process;
		
	mux: process(fsm_out,corrected_plus,corrected_minus) is
	begin
		case (fsm_out) is
			when "00" => 
				code <= "00100011";
			when "01" =>
				code <= corrected_plus;
			when "10" =>
				code <= corrected_minus;
			when others =>
				code <= "00100011";
		end case; 
	end process;
    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.
	
	

end behavioral;
