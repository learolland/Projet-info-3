; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le pair? fonctionne

(pair? (2 3))
(pair? (quote a)
(pair? ('kflf'))
(pair? ())
(pair? 3)
(pair? 'dkdk')