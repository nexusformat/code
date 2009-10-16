
proc fixrainbow {} {
    set f [open Rainbow.bck r]
    for {set i 0} {$i < 256} {incr i} {
	set r [gets $f]
	set g [gets $f]
	set b [gets $f]
	puts stdout "[string trim $r] [string trim $g] [string trim $b]" 
    }
    close $f
}
fixrainbow
