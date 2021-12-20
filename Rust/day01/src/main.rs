use itertools::Itertools;

const INPUT: &str = include_str!("../Input.txt");

fn main() {
    let mut prev_line = None;
    let mut count = 0;
    for line in INPUT.lines() {
        let parsed = match line.parse::<usize>() {
            Ok(p) => p,
            Err(_) => continue,
        };
        if let Some(prev) = prev_line {
            if prev < parsed {
                count += 1;
            }
        }
        prev_line = Some(parsed);
    }

    println!("{}", count);

    let sums = INPUT
        .lines()
        .filter_map(|line| line.parse::<usize>().ok())
        .tuple_windows()
        .map(|(a, b, c)| a + b + c)
        .collect::<Vec<_>>();

    println!(
        "part 2: {}",
        sums.iter()
            .tuple_windows()
            .map(|(a, b)| (b > a) as usize)
            .sum::<usize>()
    );
}
