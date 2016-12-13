; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le symbol->string fonctionne


(symbol->string 'test)
(symbol->string "test")
(symbol->string (quote test1) (quote test2))
