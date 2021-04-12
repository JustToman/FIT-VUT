proc isim_script {} {

   add_divider "Signals of the Vigenere Interface"
   add_wave_label "" "CLK" /testbench/clk
   add_wave_label "" "RST" /testbench/rst
   add_wave_label "-radix ascii" "DATA" /testbench/tb_data
   add_wave_label "-radix ascii" "KEY" /testbench/tb_key
   add_wave_label "-radix ascii" "CODE" /testbench/tb_code

   add_divider "Vigenere Inner Signals"
   add_wave_label "" "state" /testbench/uut/state
   add_wave_label "-radix dec" "SHIFT_BY" /testbench/uut/shift_by
   add_wave_label "-radix ascii" "CORRECTED_PLUS" /testbench/uut/corrected_plus
   add_wave_label "-radix ascii" "CORRECTED_MINUS" /testbench/uut/corrected_minus
   add_wave_label "" "FSM_OUT" /testbench/uut/fsm_out
   add_wave_label "" "current_state" /testbench/uut/current_state
   add_wave_label "" "next_state" /testbench/uut/next_state

   


   # sem doplnte vase vnitrni signaly. chcete-li v diagramu zobrazit desitkove
   # cislo, vlozte do prvnich uvozovek: -radix dec

   run 8 ns
}
