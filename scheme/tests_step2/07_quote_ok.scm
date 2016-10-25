; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que quote fonctionne et que 'a renvoie la même chose que (quote a)

(quote a)
'a
(quote ())
'()
(quote "salut")
(quote 13)
'13
(quote -3)
'-3
(quote (10 24))
'(10 24)
(quote ("salut" #f))
'("salut" #f)
(quote #t)
'#t
(quote #f)
(quote #\a)
'#\a
(quote #\space)
'#\space
(quote #\newline)
(quote ( -13 #\newline "bonjour"))
