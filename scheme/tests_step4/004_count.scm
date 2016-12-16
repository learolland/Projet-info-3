; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie qu'une commande telle que count dans le sujet fonctionne

(define count
    (
        (lambda (total)
            (lambda (increment) (set! total (+ total increment)) total)
        ) 0
    )
)
(count 3)
(count 5)
total
