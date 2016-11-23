; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le nil? fonctionne

(nil? ())
(nil? 2)
(nil? "chaine")
(define a ())
(nil? a)
