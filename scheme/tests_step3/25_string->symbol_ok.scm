; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le string->symbol fonctionne

(string->symbol "test")
(string->symbol 3)
(string->symbol "test1" "test2")
