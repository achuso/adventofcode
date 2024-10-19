const fs = require('fs');
const path = require('path');

// File path
const inputFilePath = path.join(__dirname, 'day4_input.txt');

// Calculate points for a single card (Part 1)
function calculateCardPoints(winningNumbers, ourNumbers) {
    const winningSet = new Set(winningNumbers);

    const matches = ourNumbers.filter(num => winningSet.has(num));
    const matchCount = matches.length;

    return matchCount === 0 ? 0 : Math.pow(2, matchCount - 1);
}

// Calculate matching numbers (Part 2)
function getMatchCount(winningNumbers, ourNumbers) {
    const winningSet = new Set(winningNumbers);
    const matches = ourNumbers.filter(num => winningSet.has(num));
    return matches.length;
}

// Main fn
function processScratchcards(filePath) {
    try {
        // Read file
        const fileContent = fs.readFileSync(filePath, 'utf-8');
        const lines = fileContent.trim().split('\n');

        const cards = [];
        let totalPoints = 0;

        for (const line of lines) {
            // Skip empty lines
            if (line.trim() === '')
                continue;

            // Split cards into winning numbers and our numbers
            const parts = line.split(':');
            if (parts.length < 2) {
                console.error(`Invalid card format.`);
                continue;
            }

            const cardNumberMatch = parts[0].match(/Card\s+(\d+)/);
            if (!cardNumberMatch) {
                console.error(`Invalid card number format: ${parts[0]}`);
                continue;
            }

            const cardNumber = parseInt(cardNumberMatch[1], 10);

            const cardData = parts[1].trim().split('|');
            if (cardData.length !== 2) {
                console.error(`Invalid card data.`);
                continue;
            }

            const winningNumbersStr = cardData[0].trim();
            const ourNumbersStr = cardData[1].trim();

            // Convert numbers to arrays of integers
            const winningNumbers = winningNumbersStr.split(/\s+/).map(Number);
            const ourNumbers = ourNumbersStr.split(/\s+/).map(Number);

            // Store card data
            cards.push({
                cardNumber,
                winningNumbers,
                ourNumbers,
            });

            // Calculate points for given cart
            const points = calculateCardPoints(winningNumbers, ourNumbers);
            totalPoints += points;
        }

        // Output for Part 1
        console.log(`Part 1 - Total points: ${totalPoints}`);

        // Output for Part 2
        const totalScratchCards = calculateTotalScratchCards(cards);
        console.log(`Part 2 - Total scratchcards: ${totalScratchCards}`);
    } 
    catch (error) {
        console.error(`Error reading or processing the file.`);
    }
}

// Fn to calculate total scratchcards (Part 2)
function calculateTotalScratchCards(cards) {
    const cardCount = cards.length;

    // Memoization
    const memo = new Map();

    let totalScratchCards = 0;

    // Process each original card
    for (let i = 0; i < cardCount; i++) {
        const cardNumber = i + 1;
        totalScratchCards += processCard(cardNumber, cards, memo);
    }

    return totalScratchCards;
}

// Process a card and return total scratchcards resulting from it
function processCard(cardNumber, cards, memo) {

    if (memo.has(cardNumber))
        return memo.get(cardNumber);

    const cardIndex = cardNumber - 1;
    const card = cards[cardIndex];

    let totalScratchCards = 1;

    // Get the number of matching numbers
    const matchCount = getMatchCount(card.winningNumbers, card.ourNumbers);

    // If matchCount > 0, win copies of the next matchCount cards
    if (matchCount > 0) {
        for (let i = 1; i <= matchCount; i++) {
            const nextCardNumber = cardNumber + i;
            if (nextCardNumber <= cards.length)
                // Recursively process the next card
                totalScratchCards += processCard(nextCardNumber, cards, memo);
        }
    }

    // Store the result
    memo.set(cardNumber, totalScratchCards);

    return totalScratchCards;
}

// Voila
processScratchcards(inputFilePath);
