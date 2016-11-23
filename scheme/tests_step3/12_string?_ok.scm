; TEST_RETURN_CODE=PASS
; TEST_COMMENT= vérifie que le string? fonctionne

(string? 'a')
(string? 'test')
(define c 'test')
(string? c)
(string? 3)
(string? -2)
(string? test)
