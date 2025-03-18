class Solution:
    def __init__(self) -> None:
        self.file: __file__ = open("day4.txt", "r")
        self.data: list[str] = self.file.read().strip().split('\n')
        
    def __del__(self) -> None:
        self.file.close()
        
    def sol1(self) -> int:
        if not self.data or not self.data[0]:
            return 0
            
        rows: int = len(self.data)
        cols: int = len(self.data[0])
        
        target_word: str = "XMAS"
        word_length: int = len(target_word)
        occurrences: int = 0
        
        directions: tuple[(int,int)] = [
            (-1, -1), (-1, 0), (-1, 1),
            (0, -1),           (0, 1),
            (1, -1),  (1, 0),  (1, 1)
        ]
        
        for row_idx in range(rows):
            for col_idx in range(cols):
                for row_step, col_step in directions:
                    # Direction cheeeeck
                    end_row: int = row_idx + (word_length - 1) * row_step
                    end_col: int = col_idx + (word_length - 1) * col_step
                    
                    if end_row < 0 or end_row >= rows or end_col < 0 or end_col >= cols:
                        continue
                    
                    # Check for matching word
                    is_match: bool = True
                    for char_idx in range(word_length):
                        next_row: int = row_idx + char_idx * row_step
                        next_col: int = col_idx + char_idx * col_step
                        
                        if self.data[next_row][next_col] != target_word[char_idx]:
                            is_match = False
                            break
                    
                    if is_match:
                        occurrences += 1
        
        return occurrences
    
    
    def sol2(self) -> int:
        rows: int = len(self.data)
        cols: int = len(self.data[0]) if rows > 0 else 0
        count: int = 0

        for row in range(1, rows - 1):
            for col in range(1, cols - 1):
                # Center must be 'A'
                if self.data[row][col] != 'A':
                    continue

                # Diagonal adjagency
                top_left: str = self.data[row - 1][col - 1]
                top_right: str = self.data[row - 1][col + 1]
                bottom_left: str = self.data[row + 1][col - 1]
                bottom_right: str = self.data[row + 1][col + 1]

                diag1: set[str] = {top_left, bottom_right}
                diag2: set[str] = {top_right, bottom_left}

                if diag1 == {'M', 'S'} and diag2 == {'M', 'S'}:
                    count += 1

        return count


def main():
    ans = Solution()
    print(f"Part1 ans: {ans.sol1()}", f"Part2 ans: {ans.sol2()}", sep="\n")

if __name__ == "__main__":
    main()