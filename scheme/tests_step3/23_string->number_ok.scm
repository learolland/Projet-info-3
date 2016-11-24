; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le string->number fonctionne

(string->number "3")
(string->number "test")
(string->number 3)
(string->number "1" "2")
