; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le and fonctionne

(and #t #t)
(and #f #f)
(and #t #f)
(and #f #t)
(and #t #t #t)
(and #t #t #f)
(and #f #f #t)
(and #f 22)
(and 23 #f)
(and 23 #t)
(and 23 a)


