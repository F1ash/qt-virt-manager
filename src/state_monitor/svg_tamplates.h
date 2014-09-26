#ifndef SVG_TAMPLATES_H
#define SVG_TAMPLATES_H

#define HOST_CPU_USAGE_SVG_TEMPLATE QString("\
<?xml version='1.0' encoding='UTF-8' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN'\
    'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg'\
    xmlns:xlink='http://www.w3.org/1999/xlink'\
    xmlns:ev='http://www.w3.org/2001/xml-events'\
    width='100' height='100'>\
    <g stroke='green' stroke-width='3px'></g>\
    <rect fill='white' fill-opacity='0.6'\
        x='0' y='0' width='100%' height='100%'\
        stroke='red' stroke-width='1'/>\
    <text x='3' y='23' font-family='Monospace' font-size='18' fill='black'></text>\
</svg>")

#define GUEST_MEMORY_STATE_SVG_TEMPLATE QString("\
<?xml version='1.0' encoding='UTF-8' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN'\
    'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg'\
    xmlns:xlink='http://www.w3.org/1999/xlink'\
    xmlns:ev='http://www.w3.org/2001/xml-events'\
    width='100' height='100'>\
    <g stroke='green' stroke-width='3px'></g>\
    <rect fill='white' fill-opacity='0.6'\
        x='0' y='0' width='100%' height='100%'\
        stroke='blue' stroke-width='1'/>\
    <text x='3' y='23' font-family='Monospace' font-size='14' fill='black'></text>\
</svg>")

#endif // SVG_TAMPLATES_H
