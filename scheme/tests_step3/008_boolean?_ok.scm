; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le boolean? fonctionne

(boolean? #t)
(boolean? #f)
(boolean? #t #t #f #f #t)
(boolean? (or #t #t))
(boolean? (boolean? #t))
(define a #t)
(boolean? a)

(boolean? 2)
(boolean? 'fkfrk')
(boolean? '#t')
