; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le if sans alternative fonctionne

(if #t -13)
(if #t "bonjour")
(if #t #\space)
(if #f 22)
(if #f "bonsoir")
(if #f #\newline)
