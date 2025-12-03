PROGRAM program0
  VAR
    UP_Start AT %IX0.1 : BOOL;
    DOWN_Stop AT %IX0.2 : BOOL;
  END_VAR
  VAR
    Latch : BOOL;
  END_VAR
  VAR
    LED AT %QX0.1 : BOOL;
  END_VAR

  Latch := (Latch OR UP_Start) AND NOT(DOWN_Stop);
  LED := Latch;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION
