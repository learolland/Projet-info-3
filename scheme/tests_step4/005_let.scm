; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que let fonctionne

(let ((x 5)) (* x 5))
(let ((x 2) (y 3)) (* x y))
(let ((z 5)) (- (+ z 2) 3 2))
(define f (let ((x 3) (y 4)) (+ x y)))
f
