const std = @import("std");

pub fn main() !void {
	for (std.os.argv[1..]) |arg| {
		if (std.mem.endsWith(u8, std.mem.span(arg), ".jug")) {
		}
	}
}
