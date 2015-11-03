# #####
# s: UxU -> \setR similarity function between users
# The important thing to know is the resulting number represents how “similar” the first column is with the second column. We will use the following helper function to product the Cosine Similarity:
# # Create a helper function to calculate the cosine between two vectors
#   getCosine <- function(x,y)
#   {
#     this.cosine <- sum(x*y) / (sqrt(sum(x*x)) * sqrt(sum(y*y)))
#     return(this.cosine)
#   }

# guardare pag93 per queste cose
#
# ##### uso s per calcolare N \subset U che sarebbe il mio vicinato
# ##### Once N has been computed, the system combines the ratings of users
# in N to generate predictions for user u’s preference for an item i

#### usando la formula di wikipedia :
####### r_{xy} = 1/(n-1) sum_{i=1}^{n}(x_1 - media_x)/s_x * (y_i - media_y)/s_y
######## con s_x/y = sample standard deviation => sqrt{1/(n-1) sum_{i=1}^{n}(x_1 - media_x)^2}
######### https://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient
import math

# calculates the mean
def mean(x):
    sum = 0.0
    for i in x:
         sum += i
    return sum / len(x) 

# calculates the sample standard deviation
def sampleStandardDeviation(x):
    sumv = 0.0
    for i in x:
         sumv += (i - mean(x))**2
    return math.sqrt(sumv/(len(x)-1))

# calculates the PCC using both the 2 functions above
def pearson(x,y):
    scorex = []
    scorey = []

    for i in x: 
        scorex.append((i - mean(x))/sampleStandardDeviation(x)) 

    for j in y:
        scorey.append((j - mean(y))/sampleStandardDeviation(y))

# multiplies both lists together into 1 list (hence zip) and sums the whole list   
    return (sum([i*j for i,j in zip(scorex,scorey)]))/(len(x)-1) #  using zip to iterate over two lists in paralle


    si tratta quindi poi di implementare l'eq 2.7 pag92 che mi creera questa cavolo di 
    famosa matrice di similarita
    
    {"id_utente":{"media": int, "review":{"id_rest": int}}}