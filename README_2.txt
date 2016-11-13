LIVRABLE 2 ROLLAND - DAVID
SICOM

Notre programme fonctionne avec debug. Nous avons créé un dossier tests_step2 contenant 12 tests permettant d'exploiter
et tester au maximum le programme.

Il arrive cependant que le programme simpleUnitTest avorte des tests car il n'est pas satisfait du résultat obtenu. En effet, quand une
forme a été mal entrée, nous avons voulu retourner NULL en redonnant la main a l'utilisateur, mais lors de l'execution des tests,
repl.c et simpleUnitTest.sh s'insurgent...
Certain tests fonctionnant avec make debug ne marchent plus avec make release.

Et le make tarball fait un tar mais renvoi une erreur.
