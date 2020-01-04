library ieee; 
use ieee.std_logic_1164. all;
use ieee.std_logic_unsigned.all;

entity div_100 is
	port (	rst_n : in std_logic;
			clk	  : in std_logic;
			clk_100 : out std_logic
			); 
end div_100;

architecture behavioral of div_100 is
	signal count : std_logic_vector(6 downto 0);
	constant TOTAL_CNT: integer := 99; 
	constant HALF_CNT: integer := 50; 
	
begin
	process (rst_n,clk)
	begin
		if rst_n = '0' then
			count <= (others => '0');
			clk_100 <= '0';
		elsif (clk'event and clk = '1') then
			if count < TOTAL_CNT then
				count <= count + 1;
			else count <= (others => '0');
			end if;
			
			if (count < HALF_CNT) then
				clk_100 <= '0';
			else clk_100 <= '1';
			end if;
		end if;
	end process;
	
end behavioral;
