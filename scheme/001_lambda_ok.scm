; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que la forme lambda fonctionne

( ( lambda (x) (+ x 2) ) 3  )
( procedure? ( lambda (x) (+ x 2) ) )
( ( lambda (y) (* y 2) ) 5  )
( procedure? ( lambda (y) (* y 2) ) )
( ( lambda (a b) (* a b) ) 5 8  )
( ( lambda (z) (- (+ z 2) 3 2) 5  )
( lambda (x) (+ x 2) )
( lambda (c e) (+ e 2 c 4) )