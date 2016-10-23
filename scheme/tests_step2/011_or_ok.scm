; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le or fonctionne

(or #t #t)
(or #f #f)
(or #f #t)
(or #t #f)
(or #f #f #t)
(or #f #f #f)
(or #t #t #t)
(or #t 23)
(or 23 #t)
(or 23 #f)
(or 23 b)