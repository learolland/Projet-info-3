; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le cdr fonctionne

(cdr (cons 1 (cons 2 (cons 3 ()))))
(cdr (cons 1 2))
