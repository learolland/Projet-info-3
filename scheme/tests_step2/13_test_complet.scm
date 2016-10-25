; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que toutes les formes fonctionnent entre elles

(define x #t)
(define y #f)
x
y
(if x (define x "if juste") (define y "if faux"))
x
(if y (define x "if ne fonctionne pas avec un predicat faux") (define y "if fonctionne avec un predicat faux"))
y
(if (and #t #f) (set! x "if fonctionne mal avec and") (set! y "if fonctionne avec and") )
y
(if #t (quote "genial ca marche !") (quote "ca me fatigue..."))
