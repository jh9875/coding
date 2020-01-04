library ieee;
use ieee.std_logic_1164.all;

entity tb_counter_up_dn_7 is
end tb_counter_up_dn_7;

architecture simulation of tb_counter_up_dn_7 is
	component counter_up_dn_7
		port (  up	  : in std_logic;
				rst_n : in std_logic;
				clk   : in std_logic;
				q 	  : out std_logic_vector(2 downto 0));
	end component;
	
	constant HALF_PERIOD_100M : time := 5 ns;
	signal up    : std_logic;
	signal rst_n : std_logic;
	signal clk   : std_logic;
	signal count : std_logic_vector(2 downto 0);
	
	begin
		clk_gen : process
		begin
			while(true) loop
				clk <= '0'; wait for HALF_PERIOD_100M;
				clk <= '1'; wait for HALF_PERIOD_100M;
			end loop;
		end process;
		rst_n <= '0', '1' after 100 ns;
		up <= '0', '1' after 400 ns;
		u0: counter_up_dn_7
		port map (
					up=>up,
					rst_n=>rst_n,
					clk=>clk,
					q => count);
end simulation;