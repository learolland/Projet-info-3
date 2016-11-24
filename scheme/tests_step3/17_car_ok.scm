; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le car fonctionne

(car (1 2 3))
(car (cons 1 (cons 2 (cons 3 ()))))
(car (cons 1 2))
