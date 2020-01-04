-- week10_class2.VHD

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY week10_class2 IS
PORT(
	RESETN : IN STD_LOGIC;
	CLK : IN STD_LOGIC;
	
    BCD : IN STD_LOGIC_VECTOR(-- 핀 설정);
    a, b, c, d, e, f, g : OUT STD_LOGIC;
	PIEZO : OUT STD_LOGIC
);
END week10_class2;

ARCHITECTURE HB OF week10_class2 IS

	SIGNAL DECODE : STD_LOGIC_VECTOR(-- 핀 설정);
	
	CONSTANT CNT_DO : INTEGER RANGE 0 TO 2047 := 1910;     
	CONSTANT CNT_RAE : INTEGER RANGE 0 TO 2047 := 1701;      
	CONSTANT CNT_MI : INTEGER RANGE 0 TO 2047 := 1516;     
	CONSTANT CNT_FA : INTEGER RANGE 0 TO 2047 := 1431;     
	CONSTANT CNT_SOL : INTEGER RANGE 0 TO 2047 := 1275;     
	CONSTANT CNT_RA : INTEGER RANGE 0 TO 2047 := 1135;     
	CONSTANT CNT_SI : INTEGER RANGE 0 TO 2047 := 1011;     
	CONSTANT CNT_HDO : INTEGER RANGE 0 TO 2047 := 955; 
 
	SIGNAL REG : STD_LOGIC;
	SIGNAL CNT : INTEGER RANGE 0 TO 2047;     
	SIGNAL LIMIT : INTEGER RANGE 0 TO 2047; 

BEGIN

-- 버튼에 해당하는 음계의 소리를 내도록 지정
PROCESS(BCD) 
BEGIN     
	CASE ? IS    -- 이 부분도 수정
		WHEN "" => LIMIT <= ;    -- 도
		WHEN "" => LIMIT <= ;    -- 레
		WHEN "" => LIMIT <= ;    -- 미
		WHEN "" => LIMIT <= ;   -- 파
		WHEN "" => LIMIT <= ;    -- 솔
		WHEN "" => LIMIT <= ;     -- 라
		WHEN "" => LIMIT <= ;    -- 시
		WHEN "" => LIMIT <= ;    -- 높은 도
		WHEN OTHERS => LIMIT <= 0;  
	END CASE; 
END PROCESS; 

-- 클럭이 발생하였을 때, 
-- 주파수 생성을 위한 카운트 하는 변수 CNT를 앞에서 설정한 LIMIT까지 카운트하여 
-- REG 주파수를 생성
-- 이 코드는 건들일 필요 없음.
PROCESS(RESETN, CLK) 
BEGIN  
   IF RESETN = '0' THEN 
	    CNT <= 0;     
	    REG <= '0';   
   ELSIF CLK'EVENT AND CLK = '1' THEN  
		IF CNT >= LIMIT THEN           
			CNT <= 0;         
			REG <= NOT REG;     
	    ELSE           
			CNT <= CNT + 1;      
	    END IF;  
   END IF; 
END PROCESS; 
 
-- 세그먼트 PROCESS
 

END HB;