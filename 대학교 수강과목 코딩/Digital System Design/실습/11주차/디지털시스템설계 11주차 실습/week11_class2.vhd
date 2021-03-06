-- week11_class2.VHD

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY week11_class2 IS
	PORT(
		RESETN   : IN  STD_LOGIC;       -- RESET
		CLK      : IN  STD_LOGIC;       -- 1kHz CLOCK
		SEG_COM  : OUT STD_LOGIC_VECTOR(3 DOWNTO 0); -- 7-SEGMENT COMMON SELECT
		SEG_DATA : OUT STD_LOGIC_VECTOR(7 DOWNTO 0) -- 7-SEGMENT DATA
	);
END week11_class2;

ARCHITECTURE HB OF week11_class2 IS
	SIGNAL CNT_SCAN : INTEGER RANGE 0 TO 3; -- SCAN COUNT
	
	SIGNAL CNT : ;
	SIGNAL ONE : ;
	SIGNAL TEN : ;
	
BEGIN
	
	-- TIME COUNT
	PROCESS(CLK, RESETN)
	BEGIN
		IF RESETN = '0' THEN
			CNT <= 0;
		ELSIF CLK'EVENT AND CLK = '1' THEN
			IF CNT >= 999 THEN
				CNT <= 0;
			ELSE
				CNT <= CNT + 1;
			END IF;
		END IF;
	END PROCESS;
	
	PROCESS(CLK, RESETN)
	BEGIN
		-- 각 자라에 해당하는 숫자 설정
	END PROCESS;

	-- SEGMENT DISPLAY
	PROCESS(RESETN, CLK)
	BEGIN
		IF RESETN = '0' THEN
			SEG_COM  <= X"F";
			SEG_DATA <= X"00";
		ELSIF CLK'EVENT AND CLK = '1' THEN
			CASE CNT_SCAN IS
				WHEN 0 =>
					SEG_COM  <= X"7";   -- SEL COM1
					SEG_DATA <= X"FC";	-- 0
				WHEN 1 =>
					SEG_COM  <= X"B";   -- SEL COM2
					SEG_DATA <= X"FC";	-- 0
				WHEN 2 =>
					SEG_COM  <= X"D";   -- SEL COM3
					
				WHEN 3 =>
					SEG_COM  <= X"E";   -- SEL COM4
					
				WHEN OTHERS =>
			END CASE;
			
			-- 코드 추가
			
		END IF;
	END PROCESS;
END HB;
