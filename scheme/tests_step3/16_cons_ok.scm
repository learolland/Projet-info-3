; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le cons fonctionne

(cons 3 ())
(cons "abc" (cons 2 ()))
(cons 1 (cons 2 (cons 3 ())))
