@echo standard barber pole illusion
@correspondence_problem_demo.exe 1 1
@cls

@echo triangular spiral
@echo with lines
@correspondence_problem_demo.exe 1 0 1 0 3 16
@rem @echo with dots and lines
@rem @correspondence_problem_demo.exe 1 0 1 1 3 16
@echo with dots
@correspondence_problem_demo.exe 1 0 0 1 3 16
@cls

@echo smooth spiral
@echo with lines
@correspondence_problem_demo.exe 1 0 1 0 50 40
@echo with dots
@correspondence_problem_demo.exe 1 0 0 1 50 40
@cls

@echo smooth spiral with more loops
@echo with lines
@correspondence_problem_demo.exe 1 0 1 0 50 400
@echo with dots
@correspondence_problem_demo.exe 2 0 0 1 50 400
@cls

@correspondence_problem_demo.exe 1 0 1 0 10 50
@correspondence_problem_demo.exe 1 0 0 1 10 50

@echo spiral appearing to move down, juxtaposed with barberpole
@correspondence_problem_demo.exe 2 1 1 0 50 400
