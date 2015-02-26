all:
	gcc Hostd.c List.c FileLoader.c DevMan.c MLFQ.c MMU.c ProcMan.c Proc.c Visualizer.c -o hostd
