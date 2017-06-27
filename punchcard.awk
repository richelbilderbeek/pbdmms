#!/usr/bin/awk -f
#
# punchcard.awk - ASCII 'punchcard' graph for git log
#
# Written in September 2011 by Andreas Jaggi <andreas.jaggi@waterwave.ch>
#
# usage:$ git log | punchcard.awk
#     +-------------------------------------------------------------------------------------------------+
# Tue |                          .               .   o   .   .   o   O   O   0   O                      |
# Wed |  .                                                       .   .   .   0   O                   o  |
# Thu |          .           .               o       O               .   .   o   o   .           .   .  |
# Fri |                                      .       .       .   .   O   o   o                          |
# Sat |                                              .                   .   O   .                      |
# Sun |                  .   O                           o   .           o   o   o       o   o   .   .  |
# Mon |  .   .                   .   .       .       .           .   .   o   O   O   .   .       o   0  |
#     +-------------------------------------------------------------------------------------------------+
#       0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23 
#
BEGIN {
	days[1] = "Mon";
	days[2] = "Tue";
	days[3] = "Wed";
	days[4] = "Thu";
	days[5] = "Fri";
	days[6] = "Sat";
	days[7] = "Sun";

	for (d in days) {
		for ( h = 0; h < 24; h++ ) {
			data[days[d],h] = 0;
		}
	}
	max = 0;
}
/Date:/{
	split($5,m,":");
	data[$2,(1*m[1])] = data[$2,(1*m[1])]+1;
	if ( data[$2,(1*m[1])] > max ) {
		max = data[$2,(1*m[1])];
	}
}
END {
	printf("    +");
	for ( h = 0; h < 24; h++ ) {
		printf("----", h);
	}
	printf("-+\n");
	for (d in days) {
		printf("%s |", days[d]);
		for ( h = 0; h < 24; h++ ) {
			v = (4.0*data[days[d],h])/(max*1.0);
			if ( v <= 0 ) {
				printf("    ");
			}
			if ( (v > 0) && (v < 1) ) {
				printf("  . ");
			}
			if ( (v >= 1) && (v < 2) ) {
				printf("  o ");
			}
			if ( (v >= 2) && (v < 3) ) {
				printf("  O ");
			}
			if ( (v >= 3) ) {
				printf("  0 ");
			}
		}
		printf(" |\n")
	}
	printf("    +");
	for ( h = 0; h < 24; h++ ) {
		printf("----", h);
	}
	printf("-+\n     ");
	for ( h = 0; h < 24; h++ ) {
		printf("%- 3d ", h);
	}
	printf("\n");
}