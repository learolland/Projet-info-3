; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le interger? fonctionne

(integer? 10)
(integer? -2)
(integer? +3)
(integer? 12 4 2 0 2374 -2837 +324) 
(integer? (+ 3 2 4) (- 324 42 4) (* 32 3 2))
(integer? #t)
(integer? 'jfje')
(integer? quote)
