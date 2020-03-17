-- Quicksort -----------------------------------------------------------------------------------
--
--
------------------------------------------------------------------------------------------------

qsort :: Ord a => [a] -> [a] -- The member function "qsort" is given a list and returns a list 

qsort [] = [] -- The result of a sorting an empty list is an empty list.


-- Sort the list whose first element is x and the rest of which is called xs
-- qsort leftArray -> Sort all the elements of xs which are less than x.                  
-- qsort rightArray -> Sort all the elements of xs which are greater than or equal to x
-- Then, concatenate the results with [x] in the middle.
-- | : "Such as"
-- <- : "Drawn From"
qsort (x:xs) = qsort leftArray ++ [x] ++ qsort rightArray
    where
        leftArray  = [y | y <- xs, y < x] -- The list of all y's such that y is drawn from the list xs, and y is less than x.
        rightArray = [y | y <- xs, y >= x] -- The list of all y's such that y is drawn from the list xs, and y is less than x.


-- Mergesort -----------------------------------------------------------------------------------
--
--
------------------------------------------------------------------------------------------------

merge [] ys = ys -- Base case when dealing with first list being empty, the second list will return.
merge xs [] = xs -- Base case when dealing with the second list being empty, the first list will return.

-- Recursive case by splitting the data into two halves, calling mergesort on each half, and then merging the results. 
merge (x:xs) (y:ys) = if x <= y
                      then x : merge xs (y:ys) -- If either of the lists to be merged is empty, then it returns the other list.
                      else y : merge (x:xs) ys -- Otherwise it will choose the smaller of the two first elements and puts it first in the results. 

mergesort [] = [] -- The result of a sorting an empty list is an empty list 
mergesort [x] = [x] -- The result of a sorting a single valued list is an single valued list.

--Let "a's" be one half of the array while "b's" be the other half of the array.
-- Length of the array divide by 2 to divide the array into two different section.
-- Split "x's" at the dividing value
-- Then merge both of the arrays together to form a sorted array.
mergesort xs = let (as, bs) = splitAt (length xs `quot` 2) xs 
               in merge (mergesort as) (mergesort bs)
               
-- Main ----------------------------------------------------------------------------------------
--
--
------------------------------------------------------------------------------------------------
main = do

let a = [4, 65, 2, -31, 0, 99, 2, 83, 782, 1] -- Create array with values given from the instruction

putStrLn "Quicksort" -- State that it is a Quicksort 
print (qsort a) -- Print sorted array using Quicksort algorithm

putStrLn "Mergesort" -- State that it is a Mergesort
print (mergesort a) 
