# 
# 	Make a plot of the random numbers
#

# load the necessary libraries
library(tidyverse)
library(rhdf5)

# read the configuration file to get the output file name
read_config <- function(){
	
	con <- file("config", "r")
	N <- 0
	name <- ""
  	while ( TRUE ) {
    	line <- readLines(con, n = 1)
    	if ( length(line) == 0 ) {
	      break
	    }
	    str <- strsplit(line,split=" ")[[1]]
	    if(str[1] == "N"){
	    	N <- str[2] %>% as.numeric()
	    } else if(str[1] == "outname"){
	    	name <- str[2]
	    }    	
 	}

 	close(con)

 	outname <- paste0(name,"_N",sprintf("%i",N))

 	return(outname)

}

name <- read_config()
# the name of the hdf5 input
inname <- paste0(name,".h5")
# the name of the png output
outname <- paste0(name,".png")

# read the vectors from the hdf5 file in a dataframe for plotting
df <- data.frame(x=h5read(inname,"/vectors/X"),
					y=h5read(inname,"/vectors/Y"),
					d=h5read(inname,"/vectors/D")) %>%
		pivot_longer(1:ncol(.),values_to="value",names_to="variable")

# make the plot
p <- df %>% ggplot() + 
			geom_density(aes(value,col=variable),key_glyph="smooth") +
			scale_x_continuous("value") +
			scale_y_continuous("density") +
			scale_colour_manual(breaks=c("x","y","d"),
									values=c("grey","lightgrey","black")) +
			theme_bw() +
			theme(legend.title=element_blank()) +
			labs(title=paste("VAR(d) = ",
						df %>% filter(variable=="d") %>% pull(value) %>%
								var() %>% sprintf("%.3f",.)))

p %>% ggsave(outname,.,device="png",width=10,height=6,units="cm")