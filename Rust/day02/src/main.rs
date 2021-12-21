use std::str::FromStr;

use once_cell::sync::Lazy;
use regex::Regex;

const INPUT: &str = include_str!("../input.txt");

static LINE_REGEX: Lazy<Regex> = Lazy::new(|| Regex::new(r"(forward|up|down) (\d+)").unwrap());

enum Direction {
    Forward,
    Up,
    Down,
}

impl FromStr for Direction {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "forward" => Ok(Self::Forward),
            "up" => Ok(Self::Up),
            "down" => Ok(Self::Down),
            _ => Err(s.to_string()),
        }
    }
}

struct InputLine {
    direction: Direction,
    amount: usize,
}

struct Part1 {
    x: usize,
    y: usize,
}

impl Part1 {
    fn new() -> Self {
        Self { x: 0, y: 0 }
    }

    fn compute(&self) -> usize {
        self.x * self.y
    }
}

struct Part2 {
    x: i64,
    y: i64,
    aim: i64,
}

impl Part2 {
    fn new() -> Self {
        Self { x: 0, y: 0, aim: 0 }
    }

    fn compute(&self) -> i64 {
        self.x * self.y
    }
}

fn main() {
    let parsed_lines = INPUT
        .lines()
        .map(|line| {
            let captures = LINE_REGEX.captures(line).unwrap();
            InputLine {
                direction: Direction::from_str(&captures[1]).unwrap(),
                amount: captures[2].parse().unwrap(),
            }
        })
        .collect::<Vec<_>>();

    let part1 = parsed_lines
        .iter()
        .fold(Part1::new(), |Part1 { x, y }, line| match line.direction {
            Direction::Down => Part1 {
                x,
                y: y + line.amount,
            },
            Direction::Forward => Part1 {
                x: x + line.amount,
                y,
            },
            Direction::Up => Part1 {
                x,
                y: y - line.amount,
            },
        })
        .compute();
    println!("Part 1: {}", part1);
    let part2 = parsed_lines
        .iter()
        .fold(Part2::new(), |Part2 { x, y, aim }, line| {
            match line.direction {
                Direction::Down => Part2 {
                    x,
                    y,
                    aim: aim + line.amount as i64,
                },
                Direction::Up => Part2 {
                    x,
                    y,
                    aim: aim - line.amount as i64,
                },
                Direction::Forward => Part2 {
                    x: x + line.amount as i64,
                    y: y + aim * line.amount as i64,
                    aim,
                },
            }
        })
        .compute();
    println!("Part 2: {}", part2);
}
