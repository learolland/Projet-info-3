; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le null? fonctionne

(null? ())
(null? '()')
(null? 'dke')
(null? 2)
(define c ())
(null? c)