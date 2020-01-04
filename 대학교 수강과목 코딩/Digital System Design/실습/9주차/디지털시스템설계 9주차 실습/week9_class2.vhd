-- week9_class2.VHD

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY week9_class2 IS
PORT(
    BCD : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    a, b, c, d, e, f, g : OUT STD_LOGIC;
    SEG_COM : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    COUNT_OUT : OUT STD_LOGIC_VECTOR( -- 핀 설정  )
);
END week9_class2;

ARCHITECTURE HB OF week9_class2 IS

    SIGNAL DECODE : STD_LOGIC_VECTOR(6 DOWNTO 0);
    SIGNAL CNT_16BIT : STD_LOGIC_VECTOR( -- 핀 설정  );

BEGIN

SEG_COM <= "1111";

-- LED용 프로세스 추가






































-- LED용 프로세스 종료

PROCESS(BCD)
BEGIN
    CASE BCD IS
        WHEN "0000" => DECODE <= "1111110";
        WHEN "0001" => DECODE <= "0110000"; -- 1
        WHEN "0010" => DECODE <= "1101101"; -- 2
        WHEN "0011" => DECODE <= "1111001"; -- 3
        WHEN "0100" => DECODE <= "0110011"; -- 4
        WHEN "0101" => DECODE <= "1011011"; -- 5
        WHEN "0110" => DECODE <= "1011111"; -- 6
        WHEN "0111" => DECODE <= "1110000"; -- 7
        WHEN "1000" => DECODE <= "1111111"; -- 8







		
        WHEN OTHERS => NULL;
    END CASE;
END PROCESS;

a <= DECODE(6);
b <= DECODE(5);
c <= DECODE(4);
d <= DECODE(3);
e <= DECODE(2);
f <= DECODE(1);
g <= DECODE(0);

END HB;