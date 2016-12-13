; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que toutes les formes fonctionnent entre elles

(define x #t)
(define y #f)
x
y
(if x (define x "if juste") (define y "if faux"))
x
(if y (define x "if ne fonctionne pas") (define y "if fonctionne"))
y
(if (and #t #f) (set! x "if fonctionne mal") (set! y "if fonctionne") )
y
(if #t (quote "ca marche !") (quote "ca me fatigue..."))
