; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le char->integer fonctionne

(char->integer #\3)
(char->integer "test")
(char->integer #\1 #\2)
