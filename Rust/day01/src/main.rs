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
}
