; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le char? fonctionne

(char? #\a)
(define c #\d)
(char? c)
(char? 3)
(char? -2)
(char? 'defeg')
(char? ede)
(char? #t)