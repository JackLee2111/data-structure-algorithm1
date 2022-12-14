# Risk(yoj-812) Report
##### 2021201709 李俊霖
## 算法说明
+ 定义一个Queap类，其中含有一个单向队列和一个双向队列，用双向队列来维护一个单调队列。
+ Queap中有pop、push和getMax方法。
  + push：单向队列正常入队，双向队列入队后保证队列从头到尾不递增。
  + pop：单向队列正常出队，双向队列出队或者减少cnt值。
  + getMax：取双向队列的队头。
+ 在对每一天往前x天追溯时，将确诊数据一天天入Queap类，如果追溯天数少于Queap类中元素个数，直接将Queap中元素从队头出队，直到追溯天数大于等于Queap类中元素个数。此方法的正确性来源于第i+1天追溯的天数不会超过第i天之前的，所以Queap中元素出队对结果无影响。
+ 每一天往前追溯x天的最大确诊数目为：Queap中getMax的值。
## 时间复杂度分析
+ 输入时间复杂度O(n)
+ Queap追溯最大确诊数：由分摊分析可知，使用Queap单调队列每一次查询的时间复杂度为O(1)，因此获取所有天数之前的最大确诊数的时间复杂度为O(n)。
+ 快速排序O(nlogn)
+ 二分查找O(logn)
因此总时间复杂度为O(nlogn)。
