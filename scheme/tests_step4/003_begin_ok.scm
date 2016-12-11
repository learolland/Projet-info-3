; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que begin fonctionne

(begin 1 2 3)
(begin 2 (+ 3 1) (* 2 4 3))
(define y 0)
(begin (set! y 5) (+ y 1))
(begin (define n 33) (- n 13))
(define x 8)
(if (> x 5) (begin (set! x 5) (* x x)) (- x 5) )
(define z 5)
(begin (set! z 6) (* z z))
(define x 8)
(if (> x 5) (begin (set! x 5) (* x x) ) (- x 5) )
(define x 8)
(if (> x 5) (begin ( - (begin (set! x 5) (* x x)) 5) ) )
