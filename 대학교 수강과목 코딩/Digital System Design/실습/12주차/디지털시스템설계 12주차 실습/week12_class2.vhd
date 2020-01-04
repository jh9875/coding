-- week12_class2.VHD

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY week12_class2 IS
	PORT(
		RESETN   : IN  STD_LOGIC;       -- RESET
		CLK      : IN  STD_LOGIC;       -- 1kHz CLOCK
		SEG_COM  : OUT STD_LOGIC_VECTOR(3 DOWNTO 0); -- 7-SEGMENT COMMON SELECT
		SEG_DATA : OUT STD_LOGIC_VECTOR(7 DOWNTO 0); -- 7-SEGMENT DATA
		BCD : -- 코드 추가 --
	);
END week12_class2;

ARCHITECTURE HB OF week12_class2 IS
	SIGNAL CNT_SCAN : INTEGER RANGE 0 TO 3; -- SCAN COUNT
	
	SIGNAL CNT : INTEGER RANGE 0 TO 999;
	SIGNAL ONE : INTEGER RANGE 0 TO 10;
	SIGNAL TEN : INTEGER RANGE 0 TO 10;
	
	SIGNAL BCD_STATE : -- 코드 추가 --
	
BEGIN
	
	PROCESS(BCD)
	BEGIN
	
		-- 코드 추가 --
		-- BCD 버튼 입력에 따라 BCD의 상태를 저장
		
	END PROCESS;
	
	-- TIME COUNT
	
	-- 코드 수정 --
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
	
	-- 코드 수정 --
	PROCESS(CLK, RESETN)
	BEGIN
		IF RESETN = '0' THEN
			ONE <= 0;
			TEN <= 0;
		ELSIF CLK'EVENT AND CLK = '1' THEN
			IF CNT = 999 THEN
				ONE <= ONE + 1;
			END IF;
			
			IF ONE = 10 THEN
				TEN <= TEN + 1;
				ONE <= 0;
			END IF;
			
			IF TEN = 10 THEN
				TEN <= 0;
			END IF;
		END IF;
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
					CASE TEN IS
						WHEN 0 =>
							SEG_DATA <= X"FC";	-- 0
						WHEN 1 =>
							SEG_DATA <= X"60";	-- 1
						WHEN 2 =>
							SEG_DATA <= "11011010";	-- 2
						WHEN 3 =>
							SEG_DATA <= "11110010";	-- 3
						WHEN 4 =>
							SEG_DATA <= "01100110";	-- 4
						WHEN 5 =>
							SEG_DATA <= "10110110";	-- 5
						WHEN 6 =>
							SEG_DATA <= "10111110";	-- 6
						WHEN 7 =>
							SEG_DATA <= "11100000";	-- 7
						WHEN 8 =>
							SEG_DATA <= "11111110";	-- 8
						WHEN 9 =>
							SEG_DATA <= "11110110";	-- 9
						WHEN OTHERS =>
					END CASE;
				WHEN 3 =>
					SEG_COM  <= X"E";   -- SEL COM4
					CASE ONE IS
						WHEN 0 =>
							SEG_DATA <= X"FC";	-- 0
						WHEN 1 =>
							SEG_DATA <= X"60";	-- 1
						WHEN 2 =>
							SEG_DATA <= "11011010";	-- 2
						WHEN 3 =>
							SEG_DATA <= "11110010";	-- 3
						WHEN 4 =>
							SEG_DATA <= "01100110";	-- 4
						WHEN 5 =>
							SEG_DATA <= "10110110";	-- 5
						WHEN 6 =>
							SEG_DATA <= "10111110";	-- 6
						WHEN 7 =>
							SEG_DATA <= "11100000";	-- 7
						WHEN 8 =>
							SEG_DATA <= "11111110";	-- 8
						WHEN 9 =>
							SEG_DATA <= "11110110";	-- 9
						WHEN OTHERS =>
					END CASE;
				WHEN OTHERS =>
			END CASE;
			
			IF CNT_SCAN >= 3 THEN
				CNT_SCAN <= 0;
			ELSE
				CNT_SCAN <= CNT_SCAN + 1;
			END IF;
		END IF;
	END PROCESS;
END HB;
