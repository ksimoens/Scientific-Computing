# Plotting script

# load the necessary libraries
library(tidyverse)
library(ggpubr)
library(rhdf5)

# read the hdf5 output files
list_output <- list.files("output",full.names=TRUE)

# get the number of points for each hdf5 file
Nlist <- rep(0,length(list_output))
for(i in 1:length(list_output)){

	N <- strsplit(list_output[i],split="N")[[1]][2]
	N <- strsplit(N,split="[.]")[[1]][1] %>% as.numeric()
	
	Nlist[i] <- N

}

# get the maximum and minimum number of points
i_max <- which(Nlist==max(Nlist))
i_min <- which(Nlist==min(Nlist))

# get the full vectors for the maximum and minimum number of points
# h5read from the rhdf5 library to read hdf5 files
df_out <- rbind(data.frame(N=Nlist[i_max],x=h5read(list_output[i_max],"/vectors/X"),
								f=h5read(list_output[i_max],"/vectors/F")),
				data.frame(N=Nlist[i_min],x=h5read(list_output[i_min],"/vectors/X"),
								f=h5read(list_output[i_min],"/vectors/F"))) 

# plot the function realisations for min and max
p_func <- df_out  %>% ggplot() + 
			geom_line(aes(x=x,y=f,col=factor(N))) +
			scale_x_continuous(name="x",breaks=c(0,pi/8,pi/4,3*pi/8,pi/2),
								labels=c("0","π/8","π/4","3π/8","π/2")) +
			scale_y_continuous(name=bquote(e^x~cos(x))) +
			scale_colour_manual(name="N",values=c("black","red"),
									labels=c(bquote(N==2^.(log2(min(df_out$N)))),
												bquote(N==2^.(log2(max(df_out$N)))))) +
			theme_bw() +
			theme(legend.title=element_blank(),
					legend.position="inside",
					legend.position.inside=c(0.3, 0.25),
					legend.box.background=element_rect(color="black"))

# the exact value
exact <- (exp(pi/2)-1)/2

# read the summary file and calculate the errors
df_sum <- read.csv("summary.csv") %>% mutate(eps=abs(iTRAP/exact-1)) %>%
			mutate(absI=abs(iTRAP-iTRAPjl)) %>%
			mutate(epsjl=abs(iTRAPjl/exact-1))

# fit the error dependence on N
lm_fit <- lm(data=df_sum %>% filter(N < 500000),log10(eps)~log2(N))
slope <- lm_fit$coefficients[2] %>% as.numeric()
inter <- lm_fit$coefficients[1] %>% as.numeric()

# plot the fit
df_fit <- data.frame(x=2^seq(log2(min(df_sum$N)),log2(max(df_sum$N)),length=100)) %>%
			mutate(y=10^(inter)*x^(slope/log10(2)))

str_slope <- sprintf("%.2f",slope/log10(2))
x_loc <- 2^((max(log2(df_sum$N))+min(log2(df_sum$N)))/2)

# plot the relative error of the C++ integration
p_eps_cpp <- df_sum %>%
			ggplot() + geom_line(data=df_fit,aes(x=x,y=y),col="red") +
			geom_point(aes(x=N,y=eps)) +
			scale_x_continuous(trans='log2', name="N",
								breaks=c(8,64,512,4096,32768,262144,2^21,2^24),
								labels=c(expression(2^3),expression(2^6),
											expression(2^9),expression(2^12),
											expression(2^15),expression(2^18),
											expression(2^21),expression(2^24))) +
			scale_y_continuous(name=bquote(ε[cpp]),trans="log10") +
			annotate(geom="text",x=x_loc,
						y=df_sum$eps[2],
						label=bquote(ε[cpp]~"~"~N^.(str_slope)),col="red",fontface="bold") +
			theme_bw()

lm_fit <- lm(data=df_sum %>% filter(N < 500000),log10(epsjl)~log2(N))
slope <- lm_fit$coefficients[2] %>% as.numeric()
inter <- lm_fit$coefficients[1] %>% as.numeric()

df_fit <- data.frame(x=2^seq(log2(min(df_sum$N)),log2(max(df_sum$N)),length=100)) %>%
			mutate(y=10^(inter)*x^(slope/log10(2)))

str_slope <- sprintf("%.2f",slope/log10(2))

# plot the relative error of the Julia integration
p_eps_jl <- df_sum %>%
			ggplot() + geom_line(data=df_fit,aes(x=x,y=y),col="red") +
			geom_point(aes(x=N,y=epsjl)) +
			scale_x_continuous(trans='log2', name="N",
								breaks=c(8,64,512,4096,32768,262144,2^21,2^24),
								labels=c(expression(2^3),expression(2^6),
											expression(2^9),expression(2^12),
											expression(2^15),expression(2^18),
											expression(2^21),expression(2^24))) +
			scale_y_continuous(name=bquote(ε[jl]),trans="log10") +
			annotate(geom="text",x=x_loc,
						y=df_sum$epsjl[2],
						label=bquote(ε[jl]~"~"~N^.(str_slope)),col="red",fontface="bold") +
			theme_bw()

lm_fit <- lm(data=df_sum %>% filter(absI != 0),log10(absI)~log2(N))
slope <- lm_fit$coefficients[2] %>% as.numeric()
inter <- lm_fit$coefficients[1] %>% as.numeric()

df_fit <- data.frame(x=2^seq(log2(min(df_sum$N)),log2(max(df_sum$N)),length=100)) %>%
			mutate(y=10^(inter)*x^(slope/log10(2)))

str_slope <- sprintf("%.2f",slope/log10(2))

# plot the absolute error between the C++ and Julia integrations
p_abs <- df_sum %>%
			ggplot() + 
			geom_line(data=df_fit,aes(x=x,y=y),col="red") +
			geom_point(aes(x=N,y=absI)) +
			scale_x_continuous(trans='log2', name="N",
								breaks=c(8,64,512,4096,32768,262144,2^21,2^24),
								labels=c(expression(2^3),expression(2^6),
											expression(2^9),expression(2^12),
											expression(2^15),expression(2^18),
											expression(2^21),expression(2^24))) +
			scale_y_continuous(name=bquote("|"~I[cpp]-I[jl]~"|"),trans="log10") +
			annotate(geom="text",x=x_loc,
						y=df_sum$absI[nrow(df_sum)-1],
						label=bquote(ΔI~"~"~N^.(str_slope)),col="red",fontface="bold") +
			theme_bw()

# save the plots
q <- ggarrange(p_func,p_abs,p_eps_cpp,p_eps_jl,ncol=2,nrow=2) 
q %>% ggsave("summary_plots.pdf",.,device=cairo_pdf,width=20,height=12,units='cm')