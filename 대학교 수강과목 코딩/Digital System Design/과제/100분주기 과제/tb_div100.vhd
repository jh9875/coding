library ieee;
use ieee.std_logic_1164.all;

entity tb_div_100 is
end tb_div_100;

architecture simulation of tb_div_100 is
	component div_100
		port (  rst_n : in std_logic;
				clk   : in std_logic;
				clk_100 : out std_logic);
	end component;
	
	constant HALF_PERIOD_100M : time := 5 ns;
	signal rst_n : std_logic;
	signal clk : std_logic;
	signal count : std_logic;
	
	begin
		clk_gen : process
		begin
			while(true) loop
				clk <= '0'; wait for HALF_PERIOD_100M;
				clk <= '1'; wait for HALF_PERIOD_100M;
			end loop;
		end process;
		rst_n <= '0', '1' after 100 ns;
		u0: div_100 port map (rst_n=>rst_n, clk=>clk, clk_100 => count);
end simulation;
		