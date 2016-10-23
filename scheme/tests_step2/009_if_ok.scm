; TEST_RETURN_CODE=PASS
; TEST_COMMENT= verifie le bon fonctionnement de if

(if #t 13 4)
(if #f 13 4)
(if #t +2 33)
(if #f +2 +33)
(if #t -23 -2)
(if #f -23 -2)  
(if #t -18 145)
(if #f -18 145)
(if #t -34 +14)
(if #f +32 -4)


(if #t #\a #\b)
(if #f #\a #\b)
(if #t #\newline #\space)
(if #f #\newline #\space)
(if #t #\newline #\a)
(if #f #\newline #\a)
(if #t 12 #\j)
(if #f 12 #\j)
(if #t #\space -12)
(if #f 12 #\newline)
(if #t #\a 5)
(if #f #\a 5)

(if #t "oui" "non")
(if #f "abc" "xyz")
(if #t 213 "non")
(if #f 213 "non")
(if #t "abc" 12)
(if #f "abc" 12)
(if #t "oui" #\z)
(if #f "oui" #\z)
(if #t #\newline "xyz")
(if #f "abc" #\space)
(if #t #\a "bonsoir")
(if #f #\a "bonsoir")




