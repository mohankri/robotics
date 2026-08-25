# Robotics Maths

<img width="1388" height="898" alt="image" src="https://github.com/user-attachments/assets/15f40291-ff21-416e-954c-12ac9a04bb3d" />

```
x = r · cos(a)
```


## Right-triangle solve table

Any two of the four quantities (x, y, r, a) determine the other two.

| Known | Get x | Get y | Get r | Get a |
|-------|-------|-------|-------|-------|
| r, a  | r·cos(a) | r·sin(a) | — | — |
| x, y  | — | — | hypot(x, y) | atan2(y, x) |
| r, x  | — | ±√(r² − x²) | — | ±acos(x/r) |
| x, a  | — | x·tan(a) | x / cos(a) | — |

Notes:
- The (r, x) row is sign-ambiguous — the side (left vs. right) lives in the angle.
- Prefer `atan2(y, x)` over acos/asin: it preserves the quadrant.
- cos(a) < 0 past ±90°, so x = r·cos(a) correctly reports rear detections as negative.

### cos(a) as the "how aligned" fraction

| a    | cos(a) | x        | Meaning |
|------|--------|----------|---------|
| 0°   | 1.0    | = r      | dead ahead: all range is forward distance |
| 35°  | 0.82   | 0.82·r   | mostly ahead |
| 60°  | 0.5    | half of r | as much sideways as forward |
| 90°  | 0.0    | 0        | directly beside: zero forward component |
| 120° | −0.5   | negative | behind you — the sign carries that too |
