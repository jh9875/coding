-- HB_OR2.VHD

ENTITY HB_XNOR IS
	PORT(
		A, B, C : IN BIT;
		X : OUT BIT
	);
END HB_XNOR;

ARCHITECTURE HB OF HB_XNOR IS

BEGIN
	PROCESS(A, B, C)
	BEGIN
		IF A = '0' THEN
			IF (B = C) THEN
				X <= '1';
			ELSE
				X <= '0';
			END IF;
		ELSE
			IF (B = C) THEN
				X <= '0';
			ELSE
				X <= '1';
			END IF;
		END IF;
	END PROCESS;
END HB;