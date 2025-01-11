class Solution:
    def __init__(self) -> None:
        self.file:__file__ = open("day2.txt", "r")
        self.rawdata:list[str] = [x for x in self.file.read().split("\n") if x]
        self.data:list[str] = [line.split() for line in self.rawdata]

    def __del__(self) -> None:
        self.file.close()

    def is_valid_sequence(self, nums: list[int]) -> bool:
        if len(nums) < 2:
            return True
            
        is_ascending = nums[0] < nums[1]
        
        for i in range(len(nums) - 1):
            diff = nums[i + 1] - nums[i]
            
            if not (3 >= abs(diff) >= 1):
                return False
            if not (is_ascending and diff > 0) and not (not is_ascending and diff < 0):
                return False
                
        return True

    def sol1(self) -> int:
        safe_reports = 0

        for line in self.data:
            nums = [int(item) for item in line]
            if self.is_valid_sequence(nums):
                safe_reports += 1

        return safe_reports
    
    def sol2(self) -> int:
        safe_reports = 0
        
        for line in self.data:
            nums = [int(item) for item in line]

            if self.is_valid_sequence(nums):
                safe_reports += 1
                continue
            
            for i in range(len(nums)):
                test_nums = nums[:i] + nums[i+1:] # exclude index from the list
                if self.is_valid_sequence(test_nums):
                    safe_reports += 1
                    break
                    
        return safe_reports
    
if __name__ == "__main__":
    soln = Solution()
    print(f"Part 1: {soln.sol1()}")
    print(f"Part 2: {soln.sol2()}")
    del(soln)