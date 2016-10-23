; TEST_RETURN_CODE=FAIL
; TEST_COMMENT= vérifie que set! change la valeur de la variable en entrée et ne fais rien si la variable n'existe pas

(define a 7)
a
(set! a 22)
a
(set! a "bonjour")
a
(set! b 8)
b