library(tidyverse)

#######################

# plot the wall time for the serial and parallel computation
# in function of the length of the vectors
# MPI

#######################

MPI_timings <- read.csv("MPI/MPI_timings.csv") %>%
				group_by(N) %>% summarise_all(mean) %>%
				pivot_longer(2:ncol(.),names_to="Ncores",values_to="time") %>%
				mutate(Ncores=substr(Ncores,9,9) %>% as.numeric()) %>%
				mutate(time=time*1e3)

MPI_timings$Ncores[is.na(MPI_timings$Ncores)] <- 1

ref_line <- data.frame(x=10^seq(min(log10(MPI_timings$N)),max(log10(MPI_timings$N)),length=100)) %>%
				mutate(y=10^-4*x)

p <- MPI_timings %>% ggplot() + 
			geom_line(data=ref_line,aes(x=x,y=y),col="red",linetype="dashed") +
			geom_point(aes(x=N,y=time,col=factor(Ncores))) +
			scale_x_continuous(name="vector length",trans="log10") +
			scale_y_continuous(name="wall time (ms)",trans="log10") +
			scale_colour_manual(name="MPI processes",values=c("black","#CA3C97","#F6A97A"),
									breaks=c(1,2,4)) +
			annotate(geom="text",x=10^4.5,y=10^2.5,label=bquote("time ~"~N^1),col="red",
						fontface="bold",hjust=0) +
			theme_bw() +
			theme(legend.title=element_text(size=5))

p %>% ggsave("MPI_timings.png",.,device="png",width=10,height=6,units="cm")

#######################

# plot the wall time for the serial and parallel computation
# in function of the length of the vectors
# OpenMP

#######################

OpenMP_timings <- read.csv("OpenMP/OpenMP_timings.csv") %>%
				group_by(N) %>% summarise_all(mean) %>%
				pivot_longer(2:ncol(.),names_to="Ncores",values_to="time") %>%
				mutate(Ncores=substr(Ncores,9,9) %>% as.numeric()) %>%
				mutate(time=time*1e-6)

OpenMP_timings$Ncores[is.na(OpenMP_timings$Ncores)] <- 1

ref_line <- data.frame(x=10^seq(min(log10(OpenMP_timings$N)),max(log10(OpenMP_timings$N)),length=100)) %>%
				mutate(y=10^-4*x)

p <- OpenMP_timings %>% ggplot() + 
			geom_line(data=ref_line,aes(x=x,y=y),col="red",linetype="dashed") +
			geom_point(aes(x=N,y=time,col=factor(Ncores))) +
			scale_x_continuous(name="vector length",trans="log10") +
			scale_y_continuous(name="wall time (ms)",trans="log10") +
			scale_colour_manual(name="OpenMP threads",values=c("black","#CA3C97","#F6A97A"),
									breaks=c(1,2,4)) +
			annotate(geom="text",x=10^4.5,y=10^2.5,label=bquote("time ~"~N^1),col="red",
						fontface="bold",hjust=0) +
			theme_bw() +
			theme(legend.title=element_text(size=5))

p %>% ggsave("OpenMP_timings.png",.,device="png",width=10,height=6,units="cm")

#######################

# plot the wall time for the parallel computation with different cores
# MPI and OpenMP

#######################

timings <- read.csv("core_timings.csv") %>%
			mutate(MPI=MPI,OpenMP=OpenMP/1e9) %>%
			pivot_longer(2:ncol(.),values_to="time",names_to="method")

ref_line <- data.frame(x=2^seq(min(log2(timings$Ncores)),max(log2(timings$Ncores)),length=100)) %>%
				mutate(y=10^0.25*x^-1)

p <- timings %>% ggplot() + geom_line(data=ref_line,aes(x=x,y=y),col="red",linetype="dashed") +
					geom_point(aes(x=Ncores,y=time,col=method)) +
					scale_x_continuous(name="number of cores",trans="log2",
										breaks=c(1,2,4,8,16,32)) +
					scale_y_continuous(name="wall time (s)",trans="log10") +
					scale_colour_manual(name="method",values=c("grey","black")) +
					theme_bw() +
					annotate(geom="text",x=8,y=0.5,label=bquote("time ~"~N^{-1}),
								col="red",fontface="bold") +
					theme(legend.title=element_blank(),
							legend.position="inside",
							legend.position.inside=c(0.2,0.2),
							panel.grid.minor.x=element_blank(),
							legend.background=element_rect(colour="black"))

p %>% ggsave("core_timings.png",.,device="png",width=9,height=6,units="cm")

#######################

# plot the wall time for the parallel computation with different cores
# separating the actual for loop and the distribution of memory
# MPI

#######################

timings <- read.csv("core_timings_MPI.csv") %>%
			pivot_longer(2:ncol(.),values_to="time",names_to="part")

ref_line <- data.frame(x=2^seq(min(log2(timings$Ncores)),max(log2(timings$Ncores)),length=100)) %>%
				mutate(y=10^0*x^-1)

p <- timings %>% ggplot() + geom_line(data=ref_line,aes(x=x,y=y),col="red",linetype="dashed") +
					geom_point(aes(x=Ncores,y=time,col=part)) +
					scale_x_continuous(name="number of cores",trans="log2",
										breaks=c(2,4,8,16,32)) +
					scale_y_continuous(name="wall time (s)",trans="log10") +
					scale_colour_manual(name="part",values=c("grey","black"),
										labels=c("for loop sum","distribution"),
										breaks=c("for.","distrib")) +
					theme_bw() +
					annotate(geom="text",x=8,y=0.3,label=bquote("time ~"~N^{-1}),
								col="red",fontface="bold") +
					theme(legend.title=element_blank(),
							legend.position="inside",
							legend.position.inside=c(0.25,0.2),
							panel.grid.minor.x=element_blank(),
							legend.background=element_rect(colour="black"))

p %>% ggsave("core_timings_MPI.png",.,device="png",width=9,height=6,units="cm")