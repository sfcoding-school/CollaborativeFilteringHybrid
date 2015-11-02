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
