library ieee;
use ieee.std_logic_1164.all;

entity Answer2 is
port(
    resetn : in std_logic;   -- reset
    CLK : in std_logic;    -- 1kHz Clock    
    
    lcd_e : out std_logic;
    lcd_rs : out std_logic;
    lcd_rw : out std_logic;
    lcd_data : out std_logic_vector(7 downto 0)
);
end Answer2;

-- architecture 선언 부

architecture arc of Answer2 is

-- Text LCD의 State Machine 정의

    type state is (delay, function_set, entry_mode, disp_onoff, 
                line1, line2, delay_t, clear_disp);
    signal lcd_state : state;
    signal cnt : integer range 0 to 512;
    
    signal cnt_100Hz    : integer range 0 to 4;
    signal clk_100Hz    : std_logic;

begin

-- 100Hz generate block

process(resetn, clk)
begin
    if resetn = '0' then
        cnt_100Hz <= 0;
        clk_100Hz <= '0';
    elsif clk'event and clk = '1' then
        if cnt_100Hz = 4 then
            cnt_100Hz <= 0;
            
            clk_100Hz <= not clk_100Hz;
        else
            cnt_100Hz <= cnt_100Hz + 1;
        end if;
    end if;
end process;
    
-- State Machine flow block
    
process(resetn, clk_100Hz)
begin
    if resetn = '0' then
        lcd_state <= delay;
    elsif clk_100Hz'event and clk_100Hz = '1' then
        case lcd_state is
            when delay =>
                if cnt = 70 then
                    lcd_state <= function_set;
                end if;
            when function_set =>
                if cnt = 30 then
                    lcd_state <= disp_onoff;
                end if;
            when disp_onoff =>
                if cnt = 30 then
                    lcd_state <= entry_mode;
                end if;
            when entry_mode =>
                if cnt = 30 then
                    lcd_state <= line1;
                end if;
            when line1 =>
                if cnt = 20 then
                    lcd_state <= line2;
                end if;
            when line2 =>
                if cnt = 20 then
                    lcd_state <= delay_t;
                end if;
            when delay_t =>
                if cnt = 400 then
                    lcd_state <= clear_disp;
                end if;
            when clear_disp =>
                if cnt = 200 then
                    lcd_state <= line1;
                end if;
        end case;
    end if;
end process;

process(resetn, clk_100Hz)
begin
    if resetn = '0' then
        cnt <= 0;
    elsif clk_100Hz'event and clk_100Hz = '1' then
        case lcd_state is
            when delay =>
                if cnt = 70 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when function_set =>
                if cnt = 30 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when disp_onoff =>
                if cnt = 30 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when entry_mode =>
                if cnt = 30 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when line1 =>
                if cnt = 20 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when line2 =>
                if cnt = 20 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when delay_t =>
                if cnt = 400 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            when clear_disp =>
                if cnt = 200 then
                    cnt <= 0;
                else
                    cnt <= cnt + 1;
                end if;
        end case;
    end if;
end process;

-- Text LCD set block, Text LCD display block

process(resetn, clk_100Hz)
begin
    if resetn = '0' then
        lcd_rs <= '1';
        lcd_rw <= '1';
        lcd_data <= "00000000";
    elsif clk_100Hz'event and clk_100Hz = '1' then
        case lcd_state is
            when delay =>
            when function_set =>
                lcd_rs <= '0';
                lcd_rw <= '0';
                lcd_data <= "00111100";
            when disp_onoff =>
                lcd_rs <= '0';
                lcd_rw <= '0';
                lcd_data <= "00001100";
            when entry_mode =>
                lcd_rs <= '0';
                lcd_rw <= '0';
                lcd_data <= "00000110";
            when clear_disp =>
                lcd_rs <= '0';
                lcd_rw <= '0';
                lcd_data <= "00000001";
            when delay_t =>
                lcd_rs <= '0';
                lcd_rw <= '0';
                lcd_data <= "00000010";
            when line1 =>
                lcd_rw <= '0';
                case cnt is
                    when 0 =>
                        lcd_rs <= '0';
                        lcd_data <= "10000000";
                    when 1 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 2 =>
                        lcd_rs <= '1';
                        lcd_data <= "01001100";    -- L
					when 3 =>
                        lcd_rs <= '1';
                        lcd_data <= "01101001";    -- i
                    when 4 =>
                        lcd_rs <= '1';
                        lcd_data <= "01101101";    -- m
                    when 5 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 6 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 7 =>
                        lcd_rs <= '1';
                        lcd_data <= "01101000";    -- H
                    when 8 =>
                        lcd_rs <= '1';
                        lcd_data <= "01111001";    -- y
                    when 9 =>
                        lcd_rs <= '1';
                        lcd_data <= "01100101";    -- e
                    when 10 =>
                        lcd_rs <= '1';
                        lcd_data <= "01110010";    -- r
                    when 11 =>
                        lcd_rs <= '1';
                        lcd_data <= "01111001";    -- y
                    when 12 =>
                        lcd_rs <= '1';
                        lcd_data <= "01100101";    -- e
                    when 13 =>
                        lcd_rs <= '1';
                        lcd_data <= "01101111";    -- o
                    when 14 =>
                        lcd_rs <= '1';
                        lcd_data <= "01101110";    -- n
                    when 15 =>
                        lcd_rs <= '1';
                        lcd_data <= "01100111";    -- g
                    when 16 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when others =>
                end case;        
            when line2 =>
                lcd_rw <= '0';
                case cnt is
                    when 0 =>
                        lcd_rs <= '0';
                        lcd_data <= "11000000";
                    when 1 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 2 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 3 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    --
                    when 4 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110010";    -- 2
                    when 5 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110000";    -- 0
                    when 6 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110001";    -- 1
                    when 7 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110111";    -- 7
                    when 8 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110001";    -- 1
                    when 9 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110100";    -- 4
                    when 10 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110001";    -- 1
                    when 11 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110110";    -- 6
                    when 12 =>
                        lcd_rs <= '1';
                        lcd_data <= "00110011";    -- 3
                    when 13 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 14 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 15 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    -- 
                    when 16 =>
                        lcd_rs <= '1';
                        lcd_data <= "00100000";    --                    
                    when others =>
                end case;
        end case;
    end if;
end process;
                    
lcd_e <= CLK_100HZ;

end arc;
