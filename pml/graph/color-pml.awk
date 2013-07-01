$4 == "provides" && $5 == "no" {
    # black hole.
    gsub("\'", "", $3);
    printf("/%s/s/style=rounded/style=\"rounded\",style=\"filled\"/\n", $3);
}

$4 == "requires" && $5 == "no" {
    # miracle.
    gsub("\'", "", $3);
    printf("/%s/s/style=rounded,/style=\"rounded\", style=\"filled\", fillcolor=\"cornflowerblue\", /\n", $3);
}

$4 == "has" && $5 == "no" {
    # empty.
    gsub("\'", "", $3);
    printf("/%s/s/style=rounded/style=\"rounded\", style=\"dotted\"/\n", $3);
}


$7 == "unprovided" {
    # input?
    gsub("\'", "", $5);
    printf("/\\\"r:.*%s.*\\\"/s/fontcolor=\"BLACK\"/fontcolor=\"green\"/\n", $2);
}

$7 == "not" && $8 == "consumed" {
    # output?
    gsub("\'", "", $5);
    printf("/\\\"p:.*%s.*\"/s/labelfontcolor=\"black\"/labelfontcolor=\"red\"/\n", $2);
}

$4 == "provides" && $5 == "but" && $6 == "does" && $7 == "not" && $8 == "require" {
    # transform
    gsub("\'", "", $3);
    printf("/%s/s/style=rounded/style=rounded, fontcolor=\"blue\"/\n", $3);
}



END {
    printf("/shape=plaintext/ {\n");
    printf("a\\\nlegend1 [shape=box, style=filled, label=\"grey fill - black hole\"];\n");
    printf("a\\\nlegend2 [shape=box, style=filled, color=cornflowerblue, label=\"blue fill - miracle\"];\n");
    printf("a\\\nlegend3 [shape=box, style=dotted, label=\"dotted outline - empty\"];\n");
    printf("a\\\nlegend4 [shape=plaintext, fontcolor=blue, label=\"blue text - transform\"];\n");
    printf("a\\\nlegend5 [shape=plaintext, fontcolor=red, label=\"red text - output?\"];\n");
    printf("a\\\nlegend6 [shape=plaintext, fontcolor=green, label=\"green text - input?\"];\n");
    printf("a\\\nprocess -> legend1 -> legend2 -> legend3 -> legend4 -> legend5 -> legend6 [style=invis];\n");
    printf("}\n");
}
