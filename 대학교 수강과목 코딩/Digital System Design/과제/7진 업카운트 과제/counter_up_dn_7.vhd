library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity counter_up_dn_7 is
	port (	up : in std_logic;
			rst_n , clk : in std_logic;
			q : out std_logic_vector(2 downto 0)); 
end counter_up_dn_7;

architecture behavioral of counter_up_dn_7 is
	signal count : std_logic_vector(2 downto 0) :="000";
begin
	process (rst_n,clk)
	begin
		if (rst_n = '0') then
			count <= (others => '0');
		elsif (clk'event and clk = '1') then
			if up = '1' then
				if count = "110" then
					count <= "000";
				else
					count <= count + 1 ;
				end if;
			else
				if count = "000" then
					count <= "110" ;
				else
					count <= count - 1 ;
				end if;
			end if;
		end if;
	end process;
	q(2 downto 0) <= count (2 downto 0);
end behavioral;