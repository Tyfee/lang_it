#include "../lang_it.h"

#if defined(ALL)
#define PT_EN
#endif

#ifdef PT_EN

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;


//note for myself. 
// how many verbs can actually be categorized into rules vs complete exceptions for conjugation?
// *IT WAS REVEALED TO ME IN A DREAM*, that the number of verbs in english that are actually exceptions (i'll call them isolates)
// and can't fit into hyperspecific-non-grammatical-pseudo fake rules can actually be 
// counted with less fingers than the ones in my hand 
// so someone shoot me if i'm wrong
// the only true exceptions i can think of from the top of my head are go -> went, see -> saw and of course be -> was were 
// there are no ways to apply morphology rules to reach their past tense, and there aren't any other verbs like them
// EVERY OTHER EXISTING VERB does similar operations to at least a handful of others, such as:
// draw -> drew, come -> came. (vowel change)
// think -> thought, teach ->  taught: (complex suffix appending)
// love -> loved, paint -> painted: (simple suffix appending)
// feed -> fed , meet -> met: simple vowel reduction


typedef struct
{
  const char* w;
  const char* t;
  int type;
  int plural;
} Suffix;


typedef struct {
   string word;
   string translation;
   int type;
} Word;

// this will happen at some point
// a final check for anomaly fixing
// it will store different sets of word types like 'i[4] love[3] you[11]' => {4, 3, 11};
// and a normalized frequency number like 0.7, to say whether or not it is valid
typedef struct{
     vector<int> ngram;
     float frequency;
} FrequencyTable;

vector<FrequencyTable> table = {
    { {4, 3, 0}, 1.0 },
    { {5, 2, 7}, 0.7 },
    { {4, 4, 3}, 0.3 }
};





// also for the future:
// this would be for homonyms only, i have differnt plans for polysemy;
// but stuff like banco (pra sentar) e banco (pra guardar dinheiro), PRECISA de uma camada a mais pra analise semantica
// vou guardar tipos de palavras que geralmente são encontradas em cada tipo pra apaziguar conflitos;
// manga (fruit vs clothing), laranja (fruit vs color), banco (furniture vs institution), pena (feather vs pity), cabo (yikes), grama (grass vs gram)
// like manga would definitely be a fruit if followed by verbs like "comer" ou "colher", or even shit like "de", cause suco de manga, doce de manga
// where as you cant make juice out of sleeves, and the prefere preposition would be "da" manga,
// so i think checking i-1, i-2, i, i+1, i+2 seems fine





// homonyms in portuguese

static Outcome manga_outcomes[] = {
    {"mango", 0.0f, 0}, {"sleeve", 0.0f, 0}
};
static const char* manga_tokens[] = {
    "eat", "taste", "pick", "juice", "sweet", "candy", "dessert", "flavor","taste","ripe","$",
    "shirt", "sew", "ripped", "rip", "button", "tight", "loose"
};

static Outcome rosa_outcomes[] = {
    {"pink", 0.0f, 0}, {"rose", 0.0f, 0}
};
static const char* rosa_tokens[] = {
    "color", "shirt", "clothes", "light","$",
    "flower", "button", "sprout", "thorn", "a", "an"
};


static Outcome sede_outcomes[] = {
    {"thirst", 0.0f, 0}, {"headquarters", 0.0f, 0}
};
static const char* sede_tokens[] = {
    "drink", "water", "feel","$",
    "company", "location", "building"
};

static Outcome novo_outcomes[] = {
    {"new", 0.0f, 0}, {"young", 0.0f, 0}
};
// once i implament bit flags for person/non-person. implement it too
static const char* novo_tokens[] = {
    "buy", "brand", "product", "$",
    "very", "person", "she", "he", "i",
};



static Outcome banco_outcomes[] = {
    {"bank", 0.0f, 0}, {"bench", 0.0f, 0}
};
static const char* banco_tokens[] = {
    "pay", "money", "loan", "work", "central", "national","$",
     "sit", "beautiful", "outside"
};

static Outcome pena_outcomes[] = {
    {"feather", 0.0f, 0}, {"pity", 0.0f, 0}
};
static const char* pena_tokens[] = {
    "bird", "animal", "$",
     "feel", "what", "for"
};


static Outcome bateria_outcomes[] = {
    {"battery", 0.0f, 0}, {"drums", 0.0f, 0}, {"would hit", 0.0f, 3}
};
static const char* bateria_tokens[] = {
    "phone", "charge","percentage","computer","laptop","$",
    "play", "music", "sound","loud", "$",
    "i","you"
};

static Outcome alto_outcomes[] = {
    {"tall", 0.0f, 0}, {"high", 0.0f, 0}, {"loud", 0.0f, 3}
};
static const char* alto_tokens[] = {
    "person", "he","she","i","am", "height","$",
    "", "$",
    "music","volume", "low", "turn"
};






Homonym homonyms[] = {
    {"manga", manga_outcomes, 2, manga_tokens, 13},
    {"banco", banco_outcomes, 2, banco_tokens, 7},
    {"pena", pena_outcomes, 2, pena_tokens, 6},
    {"sede", sede_outcomes, 2, sede_tokens, 7},
    {"bateria", sede_outcomes, 3, sede_tokens, 7},
    {"novo", novo_outcomes, 2, novo_tokens, 8},
    {"nova", novo_outcomes, 2, novo_tokens, 8},
    {"alto", alto_outcomes, 3, alto_tokens, 13},
    {"alta", alto_outcomes, 3, alto_tokens, 13},
    {"rosa", rosa_outcomes, 2, rosa_tokens, 11}
};

const size_t homonymCount = sizeof(homonyms) / sizeof(homonyms[0]);

// any set of (n)words in portuguese that can't be translated separately

constexpr Entry fixed_ngrams[] = {
  {"de_novo", "again"},
  {"a_gente", "we"},
  {"o_que", "what"},
  {"em_cima", "on top"},

  {"jogo_da_velha", "tic tac toe"},
  
  {"banco_de_dados", "database"}, // this needs to account for PLURAL FUCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK, LIKE BANCOS DE DADOS ARGH
  {"quanto_mais", "the more"},
  {"qual_é", "what is"},
  {"por_que", "why"},
  {"do_que", "than"},
  {"por_favor", "please"},
  {"para_sempre", "forever"},
  {"pra_sempre", "forever"},
  {"o_seu", "your"},
  {"bom_dia", "good morning"},
  {"boa_noite", "good night"},
  {"boa_tarde", "good evening"},
  {"papel_de_parede", "wallpaper"},
  {"por_toda_parte", "everywhere"},
  {"todo_lugar", "everywhere"},
  {"todo_mundo", "everybody"},
  {"ao_mesmo", "at the same"},
  {"comigo_mesmo", "with myself"},

  {"sei_lá", "i don't know"},
  {"eu_sei_lá", "i don't know"},
  {"em_casa", "at home"},
  
  {"ao_redor", "around"},
  {"perto_de", "close to"},
  {"as_vezes", "sometimes"},
  {"o_meu", "mine"},
  {"o_dele", "his"},
  {"por_causa", "because of"},
  {"o_pior", "the worst"},
  {"o_melhor", "the best"},
  {"até_mesmo", "even"},
  {"por_enquanto", "for now"},
  {"com_fome", "hungry"},
  {"com_sede", "thirsty"},
  {"com_raiva", "angry"},
  {"hoje_em_dia", "nowadays"},
  {"de_vez_em_quando", "sometimes"},
  {"se_preocup", "worry"} // need to deal with this bitch as well ..... TRUST, YOU WILL BE DEALT WITH
};
vector<string> modals = {"can", "must", "should", "could", "may", "will", "am", "is", "are"};



template <size_t N>
Suffix lookupSuff(const Suffix (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }

        if (*p == *q) {
            return dict[i];
        }
    }
    return Suffix{nullptr, nullptr, -1, -1};
}

// noun dictionary, not only nouns anymore lol
// basically every word that can't be matched with rules of breakdown will be translated directly from here
constexpr Entry nouns[] = {
  {"olá", "hello"},
  {"ola", "hello"},
  {"inglês", "english"},
  {"mundo", "world"},
  {"nome", "name"},

  {"dois", "two"},
  {"duas", "two"},
  {"três", "three"},
  {"quatro", "four"},
  {"cinco", "five"},
  {"seis", "six"},
  {"seven", "seven"},
  {"oito", "eight"},
  {"nove", "nine"},
  {"dez", "ten"},
  {"cem", "a hundred"},
  {"mil", "a thousand"},
  {"número", "number"},
  {"ano", "year"},
  {"mês", "month"},
  {"semana", "week"},
  {"hora", "hour"},
  {"leite", "milk", UNCOUNTABLE | NO_PLURAL},
  {"roda", "wheel"},
  {"blusa", "shirt"},
  {"dinheiro", "money"},
  {"palavra", "word"},

  {"terra", "dirt"}, //dirt, earth, land 
  {"praia", "beach"},

  {"escola", "school"},
  {"loja", "store"},
  

  {"morte", "death"},
  {"vida", "life"},
  {"bebê", "baby"},
  {"buraco", "hole"},
  {"espinho", "thorn"},
  
  {"banheiro", "bathroom"},
  {"cozinha", "kitchen"},
  {"sala", "room"},
  {"prédio", "building"},

  {"chocolate", "chocolate"},
  {"melancia", "watermelon"},
  {"maçã", "apple"},
  {"pêssego", "peach"},
  {"melancia", "watermelon"},

  {"natal", "christmas"},
  {"páscoa", "easter"},
  {"aniversário", "birthday"},
  {"véspera", "eve"},
  {"feriado", "holiday"},

  {"espada", "sword"},
  {"escudo", "shield"},
  {"lâmina", "blade"},
  {"armadura", "armor"},


  {"acucar", "sugar"},
  {"morango", "strawberry"},
  {"uva", "grape"},
  {"sal", "salt"},
  {"ovo", "egg"},
  {"mel", "honey"},
  {"chá", "tea"},
  {"comida", "food"},
  {"colher", "spoon"},
  {"garfo", "fork"},
  {"fogão", "stove"},
  {"receita", "recipe"},
  {"arroz", "rice"},
  {"feijão", "beans"}, // this is weird, should i store it as beans or bean? 
  {"frango", "chicken"}, 
  {"macarrão", "pasta"}, 
  {"molho", "sauce"},
  {"peixe", "fish", NO_PLURAL},
  {"cenoura", "carrot"},
  {"batata", "potato"},
  {"repolho", "cabbage"},
  {"sopa", "soup"},

  {"cachorro", "dog"},
  {"galinha", "chicken"},
  {"pássaro", "bird"},
  {"baleia", "whale"},
  {"tubarão", "shark"},
  {"gato", "cat"},
  {"macaco", "monkey"},
  {"cavalo", "horse"},
  {"burro", "donkey"},
  {"porco", "pig"},
  {"pato", "duck"},
  {"rato", "mouse"},
  {"águia", "eagle"},
  {"tamanduá", "anteater"},
  {"lobo", "wolf"},
  {"vaca", "cow"},
  {"cobra", "snake"},

  {"asa", "wing"},
  {"anjo", "angel"},
  {"ponta", "tip", ON},
  {"ponto", "point", IS_PLACE},

  {"agua", "water"}, // this is a verbifiable ig?
  {"suco", "juice"},
  {"laranja", "orange"}, // how the hell will i handle that ?
  {"porta", "door", IS_PLACE},
  {"janela", "window"},
  {"jogo", "game"}, // TODO, differentiate a noun vs the 1st person singular (um jogo vs eu jogo) // polysemy coming soon
  {"todo", "all", NO_PLURAL},
  {"cidade", "town", IS_PLACE},
  {"arma", "gun"},
  {"vida", "life"},
  {"folha", "leaf"},
  {"papel", "paper", ON},
  {"faca", "knife"},
  {"cruz", "cross"},
  {"celular", "phone", ON},
  {"tela", "screen", ON},
  {"mulher", "woman", IRREGULAR_PLURAL},
  {"homem", "man", IRREGULAR_PLURAL},
  {"garoto", "boy"},
  {"menino", "boy"},
  {"árvore", "tree"},
  {"carro", "car"},
  {"caminhão", "truck"},
  {"avião", "airplane"},
  {"ônibus", "bus"},
  {"arbusto", "bush"},
  {"garota", "girl"},
  {"menina", "girl"},
  {"filho", "son"},
  {"filha", "daughter"},
  {"pessoa", "person"},
  {"cogumelo", "mushroom"},
  {"nuvem", "cloud"}, // TODO. IRREGULAR PLURAL SUCH AS M => NS
  {"flor", "flower"},
  {"dinheiro", "money"},
  {"criança", "kid"},
  {"amigo", "friend"},
  {"fome", "hunger", UNCOUNTABLE},
  {"sede", "thirst", UNCOUNTABLE},
  {"dia", "day"},
  {"noite", "night"},
  {"manhã", "morning"},
  {"tarde", "evening"},
  {"olho", "eye"},
  {"coracao", "heart"},
  {"coisa", "thing"},
  {"estrela", "star"},
  {"livro", "book"},
  {"filme", "movie"},
  {"casa", "house"},
  {"teto", "ceiling"},
  {"parede", "wall", ON},
  {"muro", "wall", ON},
  {"chão", "floor", ON},
  {"compania", "company"},
  {"empresa", "company"},
  {"música", "music"},
  {"canção", "song"},
  {"som", "sound"},
  {"som", "sound"},
  {"sabão", "soap"},
  {"sabonete", "soap"},
  {"bolha", "bubble"},
  {"botão", "button"},
  {"sino", "bell"},
  {"campainha", "doorbell"},
  {"viagem", "trip"},

  {"manga", "mango"},
  
  {"mão", "hand", ON},
  {"pé", "foot", IRREGULAR_PLURAL},
  {"braço", "arm"},
  {"perna", "leg"},
  {"cabeça", "head"},
  {"testa", "forehead"},
  {"cabelo", "hair"},
  {"boca", "mouth"},
  {"dedo", "finger"},
  {"unha", "nail"},
  {"dente", "tooth", IRREGULAR_PLURAL},
  {"lingua", "tongue", ON},
  {"garganta", "throat"},
  {"cérebro", "brain"},
  {"coração", "heart"},
  {"pele", "skin"},
  {"osso", "bone"},


  {"principe", "prince"}, // TODO: irregular feminine noun shifts such as princesa, duquesa, garçonete, etc
  {"tradutor", "translator"},
  {"metade", "half"},
  {"meio", "middle"},
  {"bolo", "cake"},
  {"vela", "candle"},
  {"vez", "time"},
  {"tempo", "time"},
  {"gaveta", "drawer"},
  {"algo", "something"},
  {"biblioteca", "library"},
  {"detalhe", "detail"},
  {"estação", "season"},
  {"mesa", "table", ON},
  {"cama", "bed", ON},
  {"cadeira", "chair", ON},
  {"lanterna", "flashlight"},
  {"tudo", "all"},
  {"perdão", "forgiveness"},
  {"desculpa", "apology"},
  {"chapéu", "hat"},
  {"lápis", "pencil"},
  {"caneta", "pen"},
  {"bolso", "pocket"},

  
  {"ouro", "gold"},
  {"prata", "silver"},
  {"madeira", "wood"},
  {"diamante", "diamond"},

  {"lua", "moon"},
  {"sol", "sun"},
  {"vento", "wind"},
  {"fogo", "fire"},
  {"gelo", "ice"},
  {"neve", "snow"},
  {"areia", "sand"},
  {"chuva", "rain"}, // this is a verbifiable
  {"centro", "center"},

  {"pessoas", "people"},
  {"gente", "people"},
  {"nao", "not"},
  {"sim", "yes"},
  {"eis", "here's"},
  {"floresta", "forest"},
  {"selva", "jungle"},
  {"consolo", "consolation"},
  {"padaria", "bakery", IS_PLACE},
  {"mercado", "market", IS_PLACE},
  

  //slang and curse words cause i'm soooo young and hip
  
  {"caramba", "damn"},
  {"porra", "fuck"},
  {"merda", "shit"},
  {"bosta", "shit"},
  {"caralho", "fuck"}
};

constexpr Entry art[] = {
  {"o", "the"},
  {"a", "the"},
  {"um", "a"},
  {"uma", "a"}
};

constexpr Entry pre[] = { 
  {"do", "of the"},
  {"da", "of the"},
  {"de", "of"},
  {"com", "with"}, // have to differentiate this from 'como' as in 'like', how tho......... ominous
  {"sem", "without"},
  {"ou", "or"},
  {"em", "in"},
  {"no", "in the"},
  {"na", "in the"},
  {"nos", "in the"},
  {"nas", "in the"},
  {"à", "to"},
  {"às", "to the"},
  {"ao", "to the"},
   // these verbs are grounded here for misbehaving until second order  
  {"é", "is"},
  {"são", "are"},
  {"sou", "am"},
  {"foram", "were"},
  {"fomos", "were"},
  {"era", "was"},

  {"vai", "will"},
  {"vou", "will"},
  {"vamos", "will"},

  {"num", "in a"}
};

// nominative/personal pronouns
constexpr Entry pro[] = {
  {"eu", "i"},
  {"você", "you"},
  {"voce", "you"},
  {"tu", "you"},
  {"nós",  "we"},
  {"ele",  "he"},
  {"ela",  "she"},
  {"elas",  "they"},
  {"eles", "they"},
  {"esse", "this"},
  {"essa", "this"},
  {"nesse", "in this"},
  {"nessa", "in this"},
  {"esses", "these"},
  {"essas", "these"},
  {"este", "this"},
  {"esta", "this"},
  {"estes", "these"},
  {"estas", "these"},
  {"aquele", "that"},
  {"aquela", "that"},
  {"aqueles", "those"},
  {"aquelas", "those"},
  {"isso", "this"},
  {"isto", "this"},
  {"comigo", "with myself"}
  
};
constexpr Entry poss_pro[] = {
  {"seu", "your"},
  {"dela", "her"},
  {"dele",  "his"},
  {"nosso",  "our"},
  {"meu",  "my"},
  {"meus", "my"},
  {"minha",  "my"},
  {"minhas",  "my"}
};

//object pronoun match (in english)

constexpr Entry obj_pro[] = {
  {"she", "her"},
  {"he", "him"},
  {"they", "them"},
  {"you", "you"},
};



vector<string> th_per_aux = {"she", "he", "it"};
vector<string> reg_aux = {"i", "you", "we", "they"};

// oblique pronouns
constexpr Entry obl_pro[] = {
  {"te", "you"},
  {"me", "me"},
};

// adjectives
constexpr Entry adj[] = {
  {"frio", "cold"},
  {"gelado", "cold"},
  {"quente", "hot"},
  
  {"azul", "blue"},
  {"vermelho", "red"},
  {"amarelo", "yellow"},
  {"verde", "green"},
  {"preto", "black"},
  {"branco", "white"},
  {"cinza", "grey"},
  {"rosa", "pink"},
  
  
  {"primeiro", "first"},
  {"segundo", "second"},
  {"terceiro", "third"},
  {"quarto", "fourth"},
  {"quinto", "fifth"},
  {"sexto", "sixth"},
  {"sétimo", "seventh"},
  {"oitavo", "eigth"},
  {"nono", "ninth"},
  {"décimo", "tenth"},

  {"bonito", "beautiful"},
  {"lindo", "beautiful"},
  {"belo", "beautiful"},
  {"legal", "cool"},
  {"grande", "big"},
  {"forte", "strong"},
  {"fraco", "weak"},
  {"moderno", "modern"},
  {"pequeno", "little"},
  {"grande", "big"},
  {"sano", "sane"},
  {"mais", "more"},
  {"menos", "less"},
  {"engraçado", "funny"},
  {"molhado", "wet"},
  {"seco", "dry"},
  {"novo", "new"},
  {"passado", "last"},
  {"triste", "sad"},
  {"feliz", "happy"},
  {"alto", "high"},
  {"alto", "tall"},
  {"correto", "correct"},
  {"sozinho", "alone"},
  {"facil", "easy"},
  {"dificil", "hard"},
  {"bem", "well"},
  {"bom", "good"},
  {"ruim", "bad"},
  {"mal", "evil"},
  {"mau", "bad"},
  {"humido", "humid"},
  {"melhor", "better"},
  {"pior", "worse"},
  {"estranho", "weird"},
  {"esquisito", "weird"},
  {"lento", "slow"},
  {"pelado", "naked"},
  {"proprio", "own"},
  {"sério", "serious"},
  {"doente", "sick"},
  {"saudavel", "healthy"},
  {"surdo", "deaf"},
  {"cego", "blind"},
  {"certo", "right"},
  {"próprio", "own"},
  
  {"direito", "right"},
  {"esquerdo", "left"},
  {"errado", "wrong"},

  {"certo", "certain"},
  {"outro", "other"},
  {"gratis", "free"},
  {"livre", "free"},
  {"ultimo", "last"},
  {"doce", "sweet"},
  {"azedo", "sour"},
  {"amargo", "bitter"},
  {"capaz", "capable"},
  {"louco", "crazy"},
  {"doido", "crazy"},
  {"próximo", "close"},
  {"perto", "close"},
  {"longe", "far"},
  {"pesado", "heavy"},
  {"torto", "bent"},
  {"perto", "close"},
  {"limpo", "clean"},
  {"sujo", "dirty"},
  {"virgem", "virgin"}
};

//adverbs

constexpr Entry adv[] = {
  {"se", "if"},
  {"talvez", "maybe"},
  {"que", "that"},
  {"mas", "but"},
  {"quando", "when"},
  {"sobre", "about"},
  {"enquanto", "while"},
  {"quem", "who"},
  {"também", "too"},
  {"porque", "because"},
  {"onde", "where"},
  {"qual", "what is"},
  {"quantos", "how many"},
  {"e", "and"},
  {"quanto", "how much"},
  {"nunca", "never"},
  {"sempre", "always"},
  {"aqui", "here"},
  {"ali", "there"},
  {"desde", "since"},
  {"ninguém", "nobody"},
  {"fora", "out"},
  {"pouco", "bit"},
  {"até", "until"},
  {"muito", "very"},
  {"assim", "like this"},
  {"pré", "pre"},
  {"pós", "post"},
  {"já", "already"},
  {"dentro", "inside"},
  {"fora", "outside"},
  {"hoje", "today"},
  {"ontem", "yesterday"},
  {"amanhã", "tomorrow"},
  {"agora", "now"},
  {"antes", "before"},
  {"depois", "after"},
  {"para", "to"},
  {"pra", "to"},
  {"por", "for"},
  {"ainda", "still"},
  {"somente", "only"},
  {"só", "only"},
  {"apenas", "just"},
  {"então", "then"},
  {"tão", "so"}
};

struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
    bool intransitive;
};
struct VerbEnding {
    const char* ending;
    int code;
};

// verb prefixes where 0 = regular, 1 = irregular conjugation
// is it intransitive?
constexpr Verb reg_verbs[]  = {
  {"am", "lov", 0, false},
  {"gost", "lik", 0, false},
  {"quis", "want", 1, false},
  {"quer", "want", 1, false},
  {"fum", "smok", 0, true},
  {"caç", "hunt", 1, true},
  {"corr", "run", 1, true},
  {"jog", "play", 1, true},
  {"esper", "wait", 1, true},
  {"abr", "open", 1, false},
  {"fech", "clos", 0, true},
  {"molh", "wet", 1, false},
  {"pergunt", "ask", 1, true},
  {"pe", "ask", 1, false},
  {"precis", "need", 1, false},
  {"morr", "di", 0, true},
  {"sonh", "dream", 1, true},
  {"grit", "scream", 1, true},
  {"acredit","believ", 0, true},
  {"viv", "liv", 0, true},
  {"mor", "liv", 0, true},
  {"tent", "try", 1, true},
  {"compreend", "comprehend", 1, false},
  {"busc", "search", 1, false},
  {"brilh", "glow", 1, true}, // a lot of these verbs can be inferred from their nouns: brilho -> brilhar, glow -> glow
  {"possu", "have", 1, false}, // remind to implement that, me;
  {"aprend", "learn", 1, false},
  {"apag", "eras", 0, false},
  {"pint", "paint", 1, true},
  {"traduz", "translat", 0, true},
  {"mastig", "chew", 1, false},
  {"engol", "swallow", 1, false},
  {"respond", "answer", 1, false},
  
  {"desej", "wish", 1, true},
  {"trabalh", "work", 1, true},
  {"mov", "mov", 0, false},
  {"digit", "typ", 0, false},
  {"olh", "look", 1, true}, //olho -> olhar even 
  {"sint", "feel", 1, false},
  {"mat", "kill", 1, false},
  {"prefer", "prefer", 1, false},
  {"prefir", "prefer", 1, false},
  {"lembr", "remember", 1, false},
  {"par", "stop", 1, true},
  {"chor", "cry", 1, true},
  {"cur", "cure", 1, true},
  {"fing", "pretend", 1, false},
  {"ajud", "help", 1, false},
  {"desenvolv", "develop", 1, true},
  {"bast", "suffic", 0, true},
  {"beij", "kiss", 1, true},
  {"machuc", "hurt", 1, true},
  {"deslig", "turn_off", 1, true},
  {"lig", "turn_on", 1, true},
  {"empurr", "push", 1, true},
  {"pux", "pull", 1, true},
  {"apert", "press", 1, true},
  {"serv", "serv", 0, true},
  {"inici", "start", 1, false},
  {"comec", "start", 1, false},
  {"começ", "start", 1, false}
};

static const Verb* lookupRegVerb(const char* root) {
    for (const auto& v : reg_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}


constexpr Verb irr_verbs[] = {
  {"fal", "speak", 1, true},
  {"beb", "drink", 1, false},
  {"sangr", "bleed", 1, true},
  {"procri", "breed", 1, true},
  {"aliment", "feed", 1, false},
  {"sopr", "blow", 1, false},
  {"nad", "swim", 1, true},
  {"quebr", "break", 1, false},
  {"escrev", "writ", 1, true},
  {"dirig", "driv", 0, true},
  {"dirij", "driv", 0, true},
  {"est","%", 1, true}, // % is a flag for the 'to be' verb, i don't want to figure out a clever way to do that right now, so i'll simply mark it with a flag
                    // and deal with conjugation based on what's goin on around it (i - 1) and (i + 1), since it could match:
                    // está (3rd person singular => is) estamos (1st person plural => are) estão => (3rd person plural => are) or estou (1st person singular => am)

  {"cant", "sing", 1, true},
  {"danç", "danc", 0, true},
  {"ganh", "win", 1, true},
  {"volt", "go back", 1, true},
  {"te", "have", 1, false},
  {"com", "eat", 1, false},
  {"lut", "fight", 1, true},
  {"pod", "can", 1, true},
  {"consegu", "can", 1, false},
  {"consig", "can", 1, false},
  {"pos", "can", 1, false},
  {"dev", "should", 1, true},
  {"pens", "think", 1, true},
  {"v", "see", 1, false},
  {"t", "hav", 0, false},
  {"ti", "hav", 0, false},
  {"funcion", "work", 1, true},
  {"desenh", "draw", 1, true},
  {"mant", "keep", 1, true},
  {"dorm", "sleep", 1, true},
  {"durm", "sleep", 1, true},
  {"conhec", "meet", 1, true},
  {"congel", "freeze", 1, true},
  {"compr", "buy", 1, true},
  {"continu", "keep", 1, true},
  {"sa", "know", 1, true},
  {"sab", "know", 1, true},
  {"s", "know", 1, true},
  {"fa", "do", 1, false},
  {"pag", "pay", 1, true},
  {"sent", "sit", 1, true},
  {"levant", "stand_up", 1, true}, // deal with ts at some point lol, update: dealt
  {"ach", "find", 1, false},
  {"mord", "bit", 0, true}              
};

static const Verb* lookupIrrVerb(const char* root) {
    for (const auto& v : irr_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}

// 0 == pt 1 = ode 2 = ct 3 == ate 4 == ed 5 == icate 6 == ify 7 == itute 8 == er 9 = it 10 = ize
// quick dirty verb guessing
constexpr VerbEnding patt_verbs[] = {
  {"ptar", 0}, // adaptar = adapt,
  {"odir", 1}, // explodir = explode 
  {"atar", 2}, // contatar = contact
  {"trair", 2}, // extrair = extract
  {"trar", 3}, // contatar = contact // what about encontrar.... (needs more specification)
  {"ificar", 6},
  {"icar", 5},
  {"ituir", 7},
  {"erir", 8},
  {"itar", 9},
  {"izar", 20},

  //surely theres a way to imolement the past tense endings dinamically, will i? who knows
  {"ptou", 4}, {"ptei", 4}, {"ptado", 4},
  {"odiu", 4}, {"odi", 4}, {"dido", 4},
  {"tou", 4}, {"tei", 4}, {"tado", 4},
  {"traiu", 4}, {"traí", 4},
  {"trou", 4}, {"trei", 4},
  {"izou", 4}, {"izei", 4},

  //imperative ig, just adding 10 to the original
  {"pte", 10}, // adaptar = adapt,
  {"ode", 11}, // explodir = explode 
  {"ate", 12}, // contatar = contact
  {"traia", 12}, // extrair = extract
  {"tre", 13}, // contatar = contact // what about encontrar.... (needs more specification)
  {"ifique", 16},
  {"ique", 15},
  {"itua", 17},
  {"ite", 19},
  {"ize", 30},
// first person, add 20
  {"pto", 0}, {"oda", 1},
  {"ato", 2}, {"traio", 2},
  {"tro", 3}, {"ifico", 6},
  {"ico", 5}, {"ituo", 7},
  {"ito", 9}, {"izo", 20},

};

const VerbEnding* lookupEnding(const char* word) {
    for (const auto& ve : patt_verbs) {
        size_t word_len = 0;
        while (word[word_len] != '\0') ++word_len;

        size_t end_len = 0;
        while (ve.ending[end_len] != '\0') ++end_len;

        if (word_len >= end_len) {
            const char* p = ve.ending;
            const char* q = word + word_len - end_len;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == '\0') return &ve;
        }
    }
    return nullptr;
}

vector<string> infinitive = {"ar", "er", "ir", "dir", "r", "ir", "ber","zer"};
vector<string> present_non_s = {"o", "to", "go", "ro", "am", "em", "amos", "emos", "mo", "lo", "ço", "nho", "so", "ejo", "enho", "ero", "z"};
vector<string> present_s = {"a","as", "ta", "tas", "re", "ga", "ui", "uis", "ê", "ês", "em", "be", "ço"};
vector<string> general_past = {"iu", "ei","uei", "ou", "eu", "ti", "aram", "ri", "i", "imos", "inha", "is", "bia", "nha"};
vector<string> present_continuous = {"ndo", "ndo", "ando"};
vector<string> completed_past = {"ava", "ávamos", "íamos", "nhamos","ia"};
vector<string> subjunctive = {"esse", "sse", "ssemos"};
vector<string> conditional_ = {"aria", "ariamos", "eria"};
vector<string> imperative = {"e", "a", "eja", "enha", "á"};

// common suffixes with traceable trnaslation pattern
// TODO: ADD AN EXAMPLE FOR EACH CAUSE TS IS CONFUSING
constexpr Suffix suff[] = {
  {"dade", "ty", 0, 0},
  {"mente", "ly", 4, 0},
  {"ental", "ental", 1, 0},
  {"mento", "ment", 0, 0},
  {"ável", "able", 1, 0},
  {"ível", "ible", 1, 0},
  {"auro", "aur", 0, 0},
  {"erno", "ernal", 1, 0},
  {"erna", "ernal", 1, 0},
  {"ória", "ory", 1, 0},
  {"ência", "ency", 0, 0},
  {"ama", "am", 0, 0},
  {"cidade", "city", 0, 0},
  {"ogia", "ogy", 0, 0},
  {"açado", "aced", 1, 0},
  {"ágico", "agic", 1, 0},
  {"asmo", "asm", 0, 0},
  {"ágica", "agic", 1, 0},
  {"ção", "tion", 0, 0},
  {"ções", "tions", 0, 1},
  {"culo", "cle", 0, 0},
  {"cula", "cle", 0, 0},
  {"cleta", "cle", 0,0},
  {"tério", "tery",0,0},
  {"téria", "tery", 0,0},
  {"ário", "ary",0,0},
  {"ária", "ary",0,0},
  {"ral", "ral", 0,0},
  {"ial", "ial", 1,0},
  {"mal", "mal", 1,0},
  {"ais", "als", 0, 1},
  {"oria", "ory", 0, 0},
  {"ador", "ator", 0, 0},
  {"etro", "meter", 0,0},
  {"ência", "ency", 0,0},
  {"êncio", "ence", 0,0},
  {"fia", "phy", 0,0},
  {"pia", "py", 0,0},
  {"eta", "et", 0,0},
  {"ema", "em", 0,0},
  {"ndida", "ndid", 1,0},
  {"ndido", "ndid", 0, 0},
  {"fico", "fic",0,0},
  {"eito", "ect", 1,0},
  {"feita", "fect", 1,0},
  {"édia", "edy", 0,0},
  {"édio", "edy", 0, 0},
  {"ura", "ure", 0, 0},
  {"ês", "ese", 0, 0},
  {"ança", "ance", 0,0},
  {"ão", "on", 0,0},
  {"ópia", "opy", 0,0},
  {"opia", "opy", 0,0},
  {"ismo", "ism",0,0},
  {"ópico", "opic", 1,0},
  {"ópica", "opic", 1,0},
  {"arra", "ar", 0, 0},
  {"ano", "ane", 1, 0}
};

inline string script_adequation(string word) {
    auto replace_all = [&](const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = word.find(from, pos)) != std::string::npos) {
            word.replace(pos, from.size(), to);
            pos += to.size();
        }
    };

    replace_all("\xC3\xA1", "a"); // á
    replace_all("\xC3\xA0", "a"); // à
    replace_all("\xC3\xA3", "a"); // ã
    replace_all("\xC3\xA2", "a"); // â
    replace_all("\xC3\xA9", "e"); // é
    replace_all("\xC3\xAA", "e"); // ê
    replace_all("\xC3\xAD", "i"); // í
    replace_all("\xC3\xB3", "o"); // ó
    replace_all("\xC3\xB4", "o"); // ô
    replace_all("\xC3\xB5", "o"); // õ
    replace_all("\xC3\xBA", "u"); // ú
    replace_all("\xC3\xA7", "c"); // ç
replace_all("\xC3\x81", "a"); // Á
replace_all("\xC3\x80", "a"); // À
replace_all("\xC3\x83", "a"); // Ã
replace_all("\xC3\x82", "a"); // Â
replace_all("\xC3\x89", "é"); // É
replace_all("\xC3\x8A", "e"); // Ê
replace_all("\xC3\x8D", "i"); // Í
replace_all("\xC3\x93", "o"); // Ó
replace_all("\xC3\x94", "o"); // Ô
replace_all("\xC3\x95", "o"); // Õ
replace_all("\xC3\x9A", "u"); // Ú
replace_all("\xC3\x87", "c"); // Ç
    return word;

}


//normalization
//this will turn sets of letters that shift on translation and change them accordingly.
// stuff such as aceitar -> aceipt -> accept
static string normalize(string word) {
    string normalized_ = word;

    if (word.length() > 3) {
        char thirdLast = word[word.size() - 3];
        string last2 = word.substr(word.size() - 2);
        if (isVowel(thirdLast) && thirdLast != 'e' && thirdLast != 'i' && last2 == "ed") {
            normalized_ = word.substr(0, word.size() - 3) + "ed";
            
        }
            size_t pos = normalized_.find("cio");
            if (pos != std::string::npos) {
                normalized_.replace(pos, 3, "tio"); 
            }
        if (word.substr(0, 3) == "esp") {
            normalized_ = normalized_.substr(1);  
        }
         if (word.substr(0, 3) == "teo") {
              normalized_ = "theo" + normalized_.substr(3);
        }

        if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 5) == "icaly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 5) + "ically";
        }
           if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 5) == "sctly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 5) + "stly";
        }
         if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 4) == "taly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 4) + "ctly";
        }
          if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 3) == "yly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 3) + "ily";
        }
        if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 3) == "gys") {
  
            normalized_ = normalized_.substr(0, normalized_.size() - 3) + "gies";
        }
        if (normalized_.size() > 5 && normalized_.substr(normalized_.size() - 4) == "fone") {
  
            normalized_ = normalized_.substr(0, normalized_.size() - 4) + "phone";
        }

        if (normalized_.size() > 5 && normalized_.substr(0, 5) == "psico") {
            normalized_ = "psycho" + normalized_.substr(5);
        }
         if (normalized_.size() > 5 && normalized_.substr(0, 4) == "fisi") {
            normalized_ = "physi" + normalized_.substr(4);
        }
         if (normalized_.size() > 5 && normalized_.substr(0, 4) == "alfa") {
            normalized_ = "alpha" + normalized_.substr(4);
        }
      
    }

    return normalized_;
}

typedef struct{
   std::string word;
   std::string most_similar;
   float percentage;
} L_D;

L_D auto_correct(std::string word){
    L_D most_similar = {word, word, 0.0};

    string attempt;
    //qwerty
    for(int i = 0; i < word.length(); i++){
         
    }
    return most_similar;
}


//so this will get the stem of a found verb
// like chor, add o, and form it into a noun -> to cry -> a cry to cure -> a cure
// WAIT I DONT EVEN NEED THE PT?? but what about exceptions? 
// in pt its usually NOUN_FORM == 1st pers. sing. English is literally just the infinitive.
// call this when the verb comes after an article !
Word createNounFromVerb(string verb){
    string n = "";
       // this creates agents, pintor => root is pint, theres a verb entry for pint[ar] that resolves to paint. we add 'er'. we get painter
   // one who paints!
    if (verb.length() > 4) {
        string stem = "";
        string ending = "";
        string ending4 = verb.substr(verb.length() - 4);
        string ending3 = verb.substr(verb.length() - 3);
        string ending5 = verb.substr(verb.length() - 5);
        
        if (ending5 == "adora" || ending5 == "idora") { // feminine odd root, dont want to think about how to padronize this shit so IDCCCCCCCCCCC 
            stem = verb.substr(0, verb.length() - 5);
            ending = "er";
        } else if (ending4 == "ador" || ending4 == "idor") { // same but MASC4MASC
            stem = verb.substr(0, verb.length() - 4);
            ending = "er";
        } else if (ending3 == "ura") { // this will match pintura and hopefully something else? i can only think of escritura right now but the root is different so.......
            stem = verb.substr(0, verb.length() - 3); // in hindsight, is pintura an exception???? o final ura parece ser padrão para substantivos denotando qualidade de um adjetivo 
            ending = "ing";                          // like fofura. belezura. formosura. angustura. cute(ness), beauty
                                                     // since adj. comes before on the pipeline maybe they can coexist without competition

        } else if (ending3 == "ora") { // shorter ones that match most (fem.)
            stem = verb.substr(0, verb.length() - 3);
            ending = "er";
        } else if (ending3.substr(1) == "or") { // shorter ones that match most (masc.)
            stem = verb.substr(0, verb.length() - 2);
            ending = "er";
        }
        
        // If theres a verb root make it a noun
        if (!stem.empty()) {
            const Verb *v = lookupRegVerb(stem.c_str());
            const Verb *irr_v = lookupIrrVerb(stem.c_str());
            
            if (v) {
                n = v->translation + ending;
            } else if (irr_v) {
                n = irr_v->translation + ending;
            }
        }
    }
    
    return Word{verb, n, 0};
}
//maybe adjectives as well? but idk, like bless[3] -> blessed[1], curse -> cursed, (that lowk might be easier)
//does it work in pt tho?
// abençoar -> abençoado, amaldiçoar -> amaldiçoado, curar -> curado
// bless -> blessed, curse -> cursed, cure -> cured
// seems like it does
// but theres the irregular verbs of courseeeeeeeeee fuckdddddddddddd
// think of written spoken, broken, at least is literally the past tense with an "N"


Word createAdjectiveFromVerb(string verb){
  string n = "";
  return Word{n, verb, 0};
}

Word adjectification(string adj) {
    string a;
    int word_type_;

    if (adj.length() > 4) {
        if (adj.substr(adj.length() - 3) == "ado" && lookupRegVerb(adj.substr(0, adj.length() - 3).c_str())) {
            const Verb* verb = lookupRegVerb(adj.substr(0, adj.length() - 3).c_str()); // fech
            a = std::string(verb->translation) + "ed";
            word_type_ = 1;
        } else {
            a = adj;
            word_type_ = -1;
        }
    } else {
        a = adj;
        word_type_ = -1;
    }

    return Word{adj, a, word_type_};
}


// dictionary lookup

// this is about to be nuts, maybe
// we'll do basic fragmentation, using the list of preffixes, and try to find a match
// i want this to be separate from prefixLookup() cause this will be >=3 and i need to lookup more than one table
//e.g inquebrável -> unbreakable. 
// we need to match the preffix "in" to "un", remove the last vowel and check if the root exists, (quebrá -> quebr -> break).
// and get the suffix and map it "ável" -> able
// putting this all together we get unbreakable.
// preffixes that are highly productive, [in -> un], [des -> de]
//yeah i'll need to make up rules like crazy here. theres no pattern at all
// you have un, in, ir and they just work by vibes i guess, unreal, incorrect, uncanny
// like a bunch of starting with r adjectives like real, will take ir, such as irrational, irresponsible 
// but what about exceptions such as unreal? unrequited?
// without
static Word morphemeLookup(string word){
  string translation_; 
  string p;
  string m;
  string s;
  int word_type_;
  
if(word.length() > 5 && word.substr(word.length() - 5) != "ção" && word.substr(word.length() - 3) != "cao"){
       // this will lookup the basic adjective negation (?), whats even the name of this linguistically, idk
       // but like, if you find the preffix (un) and the rest of the word is an adjective
       // you just put them together: incorreto -> [in] + [correct]
        if(word.substr(0, 2) == "in" && lookup(adj, (word.substr(2, word.length()).c_str()))){
         
             p = "in";
             m = lookup(adj, (word.substr(2, word.length()).c_str()));
             translation_ = p + m;
             word_type_ = 1;
          }
          else if (word.substr(0, 2) == "in" || word.substr(0, 2) == "im")  {
    string stem = word.substr(2);  


    for (auto &entry : suff) {
        const string &suffix = entry.w;

        if (stem.size() >= suffix.size() &&
            stem.compare(stem.size() - suffix.size(), suffix.size(), suffix) == 0) {
            
            string base = stem.substr(0, stem.size() - suffix.size());
            p = "un";

            string m;
            if (lookupIrrVerb(base.c_str())) {
                m = lookupIrrVerb(base.c_str())->translation;  
            } else {
                const Verb* v = lookupRegVerb(base.c_str());
                m = v ? v->root : base; 
            }
            s = m + entry.t;               
            translation_ = p + s;          
            word_type_ = 1;
            break;
        }
    }
          }




else{
             translation_ = "";
             word_type_ = -1;
          }
  }else{
    return {word, "", -1};
  };
  

  return Word{word, translation_, word_type_};
};

    //look for preffix matches 
Word prefixLookup(string word){
    string translation = word; 
    int word_type;


    // this will try to find a verb ending that can be translated to past tense or infinitive or continuous or whatever
    // it's a lambda that returns a pair with the match lemma + the vowel/conjugation.
    auto find_verb = [](vector<string> format, string word, int verb_info) -> Word {
 
        for(size_t i = 0; i < format.size(); ++i){
            string translation_;
            int word_type_;
            int verb_type;
            bool aux = false;
            bool compound = false; // is this verb a compound one? make out, find out, sit down, pass around ets chetera!! DID YOU JUST MISPRONOUNCE ET CETERA? my latin degree was fake jeff 
            string compound_verb;
            string verb_complement;
            bool intransitiveness;
            string ending;

            size_t match = word.rfind(format[i]);
            if (match != string::npos && match + format[i].length() == word.length()) {
                string root = word.substr(0, match);
                auto v_irr = lookupIrrVerb(root.c_str());
                char buffer[64];
                const Verb* v = lookupRegVerb(root.c_str());
                
                

                if (v) {
                    const char* ending = "";
              for(int i = 0; i < string(v->translation).length(); ++i){
                        if(v->translation[i] == '_') { // if theres an underscore in the middle of the verb, then its 110% a compound verb, i would know
                    
                            compound = true;
                            compound_verb = string(v->translation).substr(0, i); // and make a copy of the first part omg
                            verb_complement = string(v->translation).substr(i + 1); // also exctract the following word (up, down, around etc)
                            break;
                        } 
                    }
                    
                 if(verb_info == 1 || verb_info == 2){ // past tense
                        string stem =  (compound ? compound_verb : string(v->translation));
                        if(!stem.empty() && stem.back() == 'y'){
                            stem.back() = 'i';   // try -> tri
                            stem += "ed";        
                        } else {
                            stem += "ed";        // regular verbs
                        }
                        return Word{word, (compound? stem + " " + verb_complement : stem), 3}; // 3 = verb past?
                    }
                    
                    switch (verb_info) {
                        case 0: ending = (v->type == 0) ? "e" : ""; break;
                        case 1: case 2: ending = "ed"; break;
                        case 3: ending = (v->type == 0) ? "es" : "s"; break;
                        case 4: ending = "e"; break;
                        default: break;
                    }
                     if (verb_info == 10) {
                        
                        const char* would = "de";
                        size_t i = 0;
                        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
                        const char* base = (compound ? compound_verb.c_str() : v->translation);
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    
                    }
                    else if (verb_info == 11) {
                       const char* would = "pre";
                        size_t i = 0;
                        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
                        const char* base = (compound ? compound_verb.c_str() : v->translation);
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    }
                    else if (verb_info == 12) {
                      const char* would = "re";
                        size_t i = 0;
                        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
                        const char* base = (compound ? compound_verb.c_str() : v->translation);
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    }
                    else if (verb_info == 3) {
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? verb_complement : "");

                        translation_ = base + ending;
                    }

                    else if (verb_info == 4) { 
                        const char* used = "used to ";
                        size_t i = 0;
                        while (*used && i + 1 < sizeof(buffer)) buffer[i++] = *used++;
                        const char* base = v->translation;
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    }
                   else if (verb_info == 5) {
                          size_t i = 0;
                          const char* base = v->translation;
                          while (*base && i + 1 < sizeof(buffer)) {
                              buffer[i++] = *base++;
                          }

                              if (i > 0 && buffer[i - 1] == 'e') {
                                i--;
                                }
                          const char* ending = "ing";
                         
                           while (*ending && i + 3 < sizeof(buffer)) buffer[i++] = *ending++;

                          buffer[i] = '\0';
                      }
                    else if(verb_info == 6){
                        const char* would = "would ";
                        size_t i = 0;
                        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
                        const char* base = (compound ? compound_verb.c_str() : v->translation);
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    }
                     
                    else{
                        size_t i = 0;
                        const char* base = (compound ? compound_verb.c_str() : v->translation);
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        const char* e = ending;
                        while (*e && i + 1 < sizeof(buffer)) buffer[i++] = *e++;
                        buffer[i] = '\0';
                    }

                    intransitiveness = v->intransitive;
                    if(intransitiveness == true){
                        word_type_ = 3;
                    }else{
                       word_type_ = 36;
                    }
                    verb_type = v->type;
                    return Word{word,(compound ? string(buffer) + " " + verb_complement : string(buffer)), word_type_};

                } else if(v_irr){
                    for(int i = 0; i < string(v_irr->translation).length(); ++i){
                        if(v_irr->translation[i] == '_') { // if theres an underscore in the middle of the verb, then its 110% a compound verb, i would know
                    
                            compound = true;
                            compound_verb = string(v_irr->translation).substr(0, i); // and make a copy of the first part omg
                            verb_complement = string(v_irr->translation).substr(i + 1); // also exctract the following word (up, down, around etc)
                            break;
                        } 
                    }
                    

                    switch (verb_info)
                    {
                        case 0:  case 10: ending = (v_irr->type == 0) ? "e" : ""; break;
                        case 1: case 2: ending = "ed"; break;
                        case 3: ending = (v_irr->type == 0) ? "es" : "s"; break;
                        case 4: ending = (v_irr->type == 0) ? "e" : ""; break;
                        case 5: ending = "ing"; break;
                        case 6: case 7: ending = (v_irr->type== 0) ? "e" : ""; break;
                        default: break;
                    }
                    if (verb_info == 10) {
                        
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? verb_complement : "");
                     
                        translation_ = "de" + base + ending; 
                    }
                    else if (verb_info == 11) {
                        
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? verb_complement : "");
                        translation_ = "pre" + base + ending; 
                    }
                    else if (verb_info == 12) {
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? verb_complement : "");
                        translation_ = "re" + base + ending; 
                    }
                    else if (verb_info == 3) {
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? verb_complement : "");

                        translation_ = base + ending;
                    }

                    else if(verb_info == 4){
                        translation_ = "used to " + (compound ? compound_verb : string(v_irr->translation)) + (compound ? (" " + verb_complement) : "") + ending;
           
                    }
                    else if (verb_info == 5) {
                        string base = (compound ? compound_verb : string(v_irr->translation)) + (compound ? (" " + verb_complement) : "");

                        if (!base.empty() && base.back() == 'e' && base != "be") {
                            base.pop_back();
                        }

                        // consonant doubling rule: if word ends consonant-vowel-consonant (and length > 2)
                        // e.g. run → running, stop → stopping, but not need → needing
                        if (base.length() >= 3) {
                            char a = base[base.length() - 3];
                            char b = base[base.length() - 2];
                            char c = base[base.length() - 1];

                            if (!isVowel(a) && isVowel(b) && !isVowel(c) && c != 'w' && c != 'x' && c != 'y') {
                                base.push_back(c);
                            }
                        }

                        // append ing
                        translation_ = base + "ing";
                    }

                    else if(verb_info == 6){
                        if (root == "pod") translation_ = "could";
                        else if(root == "dev") translation_ = "should";
                        else translation_ = "would " + (compound ? compound_verb : string(v_irr->translation)) + (compound ? (" " + verb_complement) : "") + ending;
                        aux = true;
                    } else if(verb_info == 1 || verb_info == 2){
         //TODO: Set up the very specific rules that most verbs can abide to.
                
                if(string(v_irr->translation).substr(string(v_irr->translation).length() - 3, 2) == "ee"){
                      translation_ = string(v_irr->translation).substr(0, string(v_irr->translation).length() - 2) +
                      string(v_irr->translation).substr(string(v_irr->translation).length() - 1) + 
                      (string(v_irr->translation).substr(string(v_irr->translation).length() - 1) == "p" ? "t" : "");
                    
                      // O VOWEL SHIFT + E 

                      // this is a weird ass pattern that works for a small lil list of verbs (7 as of right now T-T)
                      // if an irregular verb starts with two consonants (substr(0, 1) and substr(1, 1) dont pass isVowel())
                      // and the two consonants are followed by either 'ea', 'i' or 'ee'*
                      // BUT IT DOESNT END IN D, G, P, K or M LMAOOOOOOOOOO
                      // you basically get the vowel(s)* that follow the two consonants and replace them with an O
                    //and if it DOESNT end with an E, you add it 
                        // the vowel to o shift also works if the verb ends in 'get' (get => got, forget => forgot )
                      // this way steal => stole, break => broke, speak => spoke, drive => drove 

                } else if (string(v_irr->translation).length() >= 3 && // is the word more than three letters?
                      !isVowel(string(v_irr->translation)[0]) &&   // is the first letter not a vowel?
                      !isVowel(string(v_irr->translation)[1]) &&   // is the second letter not a vowel?
                      (string(v_irr->translation).substr(2, 2) == "ea" || //are they followed by either "ea" || "i" || "ee"
                        string(v_irr->translation).substr(2, 1) == "i" || 
                        string(v_irr->translation).substr(2, 2) == "ee") &&
                      string(v_irr->translation).back() != 'd' &&  // do they NOT end in 'd' || 'g' || 'p' || 'k'
                      string(v_irr->translation).back() != 'g' && 
                      string(v_irr->translation).back() != 'p' &&
                     string(v_irr->translation).back() != 'k' &&
                       string(v_irr->translation).back() != 'm'
                    ) {
                            size_t pos;
                            size_t length;

                            // qual das vogais é a que o verbo tem? 
                            if ((pos = string(v_irr->translation).find("ea")) != string::npos) {
                                length = 2;  
                            } else if ((pos = string(v_irr->translation).find("i")) != string::npos) {
                                length = 1; 
                            } else if ((pos = string(v_irr->translation).find("ee")) != string::npos) {
                                length = 2; 
                            } else {
                                translation_ = string(v_irr->translation);
                            }

                          translation_ = string(v_irr->translation);
                          
                          // remover as vogais e substituir por 'o'
                          translation_.replace(pos, length, "o");
                          if (translation_.back() != 'e') {
                              translation_ += "e";
                          }
                        }

            
                                 // ANOTHER HYPERSPECIFIC RULE
                            // is the verbs second letter not 'h'?
                            // does it end in either ng or nk?

                            string base_ = string(v_irr->translation);
                            if (base_.length() >= 3 
                            && base_[1] != 'h' 
                        && (base_.substr(base_.length()-2) == "ng" 
                        || base_.substr(base_.length()-2) == "nk"))
                            {
                                        // replace the i for an 'a' so that e.g: drink => drank
                                translation_ = base_.replace(base_.find("i"), 1, "a");;  
                            }
                        
                    }else {
                        translation_ =  (compound ? compound_verb : string(v_irr->translation)) + (compound ? (" " + verb_complement) : "") + ending;
                    }

                    intransitiveness = v_irr->intransitive;
                    if(intransitiveness == true){
                        word_type_ = !aux ? 3 : 33;
                    }else{
                       word_type_ = 36;
                    }
                    verb_type = v_irr->type;
                    return {word, translation_, word_type_};
                }
            }
            
        }
        
        return Word{word, "", -1};
    };

    Word result;

    result = find_verb(infinitive, word, 0);
    if(result.type != -1) return result;

    result = find_verb(present_non_s, word, 0);
    if(result.type != -1) return result;

    result = find_verb(present_s, word, 3);
    if(result.type != -1) return result;

    result = find_verb(general_past, word, 1);
    if(result.type != -1) return result;

    result = find_verb(present_continuous, word, 5);
    if(result.type != -1) return result;

    result = find_verb(completed_past, word, 4);
    if(result.type != -1) return result;

    result = find_verb(subjunctive, word, 2);
    if(result.type != -1) return result;

    result = find_verb(conditional_, word, 6);
    if(result.type != -1) return result;

    result = find_verb(imperative, word, 7);
    if(result.type != -1) return result;


    // i'll handle this better but for testing now i'll just duplicate i guess
   if (word.size() > 3) {
    if(word.substr(0, 3) == "des") {
      result = find_verb(infinitive, word.substr(3), 10);
       if (result.type != -1) return result;
     }
     else if(word.substr(0, 3) == "pre") {
      result = find_verb(infinitive, word.substr(3), 11);
       if (result.type != -1) return result;
     }
      else if(word.substr(0, 2) == "re") {
      result = find_verb(infinitive, word.substr(2), 12);
       if (result.type != -1) return result;
     }
   }
    //re 
 
    

      const VerbEnding* ve = lookupEnding(word.c_str());
      if (ve) { 
          std::string stem; 
          std::string ending; 
          size_t ending_len = std::string(ve->ending).size();
          if (ve->code == 4) { 
              stem = word.substr(0, word.size() - (ending_len - 2)); 
          } else { 
              stem = word.substr(0, word.size() - ending_len); 
          } 
          switch(ve->code) { 
              case 0: case 10: ending="pt"; break; 
              case 1: case 11: ending="ode"; break; 
              case 2: case 12: ending="act"; break; 
              case 3: case 13: ending="trate"; break; 
              case 4: case 14: ending="ed"; break; 
              case 5: case 15: ending="icate"; break; 
              case 6: case 16: ending="ify"; break; 
              case 7: case 17: ending="itute"; break; 
              case 8: case 18: ending="er"; break; 
              case 9: case 19: ending="it"; break; 
              case 20: case 30: ending="ize"; break; 
          } 
          if(stem.length() > 2) { 
              return {stem, normalize(stem + ending), 3}; 
          } else { 
              return {word, normalize(word), 3}; 
          } 
          
      }
   

    return {word, "", -1};
}
Word suffixLookup(const std::string& word) {
    std::string translation;
    int word_type = 0;

    // example: in- prefix detection
    if (word.size() > 4) {
        if (word.substr(0, 2) == "in" &&
            lookupSuff(suff, word.substr(word.size() - 4).c_str()).t) {
            return {word, "", -1};
        }
    }

    // check suffix patterns
   for (int len = 6; len >= 2; --len) {
    if (word.length() >= static_cast<size_t>(len)) {

        std::string ending = word.substr(word.length() - len);
        Suffix suffResult = lookupSuff(suff, ending.c_str());

        if (suffResult.t) {
            const char* mapped = suffResult.t;
            std::string stem = word.substr(0, word.length() - len);
            word_type = suffResult.type;
            const char* adjResult = lookup(adj, stem.c_str());
            if (adjResult) {
                translation = std::string(adjResult) + mapped;
            word_type = 1;
            }
            else if (!stem.empty()) {
                std::string altStem = stem.substr(0, stem.length() - 1) + "o";
                const char* altAdj = lookup(adj, altStem.c_str());
                if (altAdj)
                    translation = std::string(altAdj) + mapped;
                else
                    translation = stem + mapped;
            }
            else {
                translation = stem + mapped;
            }

            return {word, normalize(script_adequation(translation)), word_type};
        }
    }
}


    return {word, word, -1}; // fallback: unchanged
}

bool isDiminutive(const std::string& s, const char* suffix) {
    size_t n = 0;
    while (suffix[n] != '\0') n++; 
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; i++) {
        if (s[s.size() - n + i] != suffix[i]) return false;
    }
    return true;
}


Word nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type = -1;


  
  
  // rules
   bool plural = ((lookup(nouns, (word.substr(0, word.length() - 2)).c_str()) || lookup(nouns, (word.substr(0, word.length() - 2) + "m").c_str()) || lookup(nouns, (word.substr(0, word.length() - 1)).c_str()) || lookup(nouns, (word.substr(0, word.length() - 2) + "o").c_str())) && word.substr(word.length() - 1) == "s"); // this is plural nouns only
   bool gender_shift = lookup(nouns, (word.substr(0, word.length() - 1)  + "o").c_str()); // this is gender shift for nouns only

   // adjectives need diminutive as well
bool diminutive = false;
    if (isDiminutive(word, "inho")) {
        
         word_type = 0; 
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 4) + "o").c_str()));
    }
    
    else if (isDiminutive(word, "inha")) {
     
        word_type = 0; 
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 4) + "a").c_str()));
    }
    else if (isDiminutive(word, "inhos")) {
        
        word_type = 0; 
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "o").c_str()));
    }
    else if (isDiminutive(word, "inhas")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "a").c_str()));
    }
    else if (isDiminutive(word, "zinho")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "o").c_str()));
    }
    else if (isDiminutive(word, "zinha")) {
         diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "a").c_str()));
    }
    else if (isDiminutive(word, "zinhos")) {
      diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 6) + "o").c_str()));
    }
    else if (isDiminutive(word, "zinhas")) {
         diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 6) + "a").c_str()));
    }
   bool adj_plural = ((lookup(adj ,(word.substr(0, word.length() - 1).c_str())) || lookup(adj, (word.substr(0, word.length() - 2) + "o").c_str())) && word.substr(word.length() - 1) == "s"); // this is plural adjectives only
   bool adj_gender_shift = lookup(adj, (word.substr(0, word.length() - 1)  + "o").c_str()); // this is gender shift for adjectives only
   bool article_plural = lookup(art, (word.substr(0, word.length() - 1).c_str()));
  

  // for each individual word loop, you look in the noun dictionary
  //first with accentuation, 
  if(lookup(nouns, word.c_str())){
   translation = lookup(nouns, word.c_str());
   word_type = 0;
   }
   //then without accentuation (helpful in plural)
   else if(lookup(nouns, script_adequation(word).c_str())){
       translation = lookup(nouns, script_adequation(word).c_str());
       word_type = 0; 
   }
   else if(lookup(adj, (script_adequation(word)).c_str())){
    
      translation = lookup(adj, script_adequation(word).c_str());
      word_type = 1;

    }
    else if(lookup(pro, word.c_str())){
      translation = lookup(pro, word.c_str());
      word_type = 4;
    } else if(lookup(poss_pro, word.c_str())){
      translation = lookup(poss_pro, word.c_str());
      word_type = 40;
    }
  
    else if(lookup(obl_pro, word.c_str())){
      translation = lookup(obl_pro, word.c_str());
      word_type = 11;
    }

     else if(lookup(pre, word.c_str())){
      translation = lookup(pre, word.c_str());
      word_type = 8;
    }//preposition plurals are simple i guess
    else if(lookup(pre, word.substr(0, word.length() - 1).c_str())){
      translation = lookup(pre, word.substr(0, word.length() - 1).c_str());
      word_type = 8;
    }
     else if(lookup(art, word.c_str())){
      translation = lookup(art, word.c_str());
      word_type = 9;
    }
     else if(article_plural){
      translation = lookup(art, (word.substr(0, word.length() - 1).c_str()));
      word_type = 9;
    }
    
     else if(lookup(adv, word.c_str())){
      translation = lookup(adv, word.c_str());
      word_type = 13;
    }
    
    
  else if(plural){
    // by removing the last letter of the word, we can check for **BASIC** plural. e.g casa[s] -> casa
    //if the noun ends in f or fe, we substitute for ves, life -> lives, leaf => leaves
    string singular_pt;
    string word_normalized = script_adequation(word);
 
    if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "os") {
        singular_pt = word.substr(0, word.size() - 2) + "o"; // gatos -> gato, cachorros -> cachorro
    } else if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "as") {
        singular_pt = word.substr(0, word.size() - 2) + "a"; // casas -> casa, cachorras -> cachorra, crianças -> criança
               std::string key = singular_pt.substr(0, singular_pt.size() - 1) + "o";
                    if(lookup(nouns, key.c_str())){
                        singular_pt = key; 
                    }
    } else if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 3) == "res") { // i need to know the other three-letter-plurals, i know z, like vezes
        singular_pt = word.substr(0, word.size() - 3) + "r";       // flores -> flor
    }else if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "ns") { 
        singular_pt = word.substr(0, word.size() - 2) + "m";       // nuvens -> nuvem
    }
     else if (word_normalized.size() > 1 && word_normalized.back() == 's') {
        singular_pt = word.substr(0, word.size() - 1);       // fallback para palavras terminadas em s
    } else {
        singular_pt = ""; // não é plural conhecido
    }

    if (lookup(nouns, singular_pt.c_str())) {
        string singular_en = lookup(nouns, singular_pt.c_str());
        if (!singular_en.empty()) {
            if (singular_en.size() >= 2 && singular_en.substr(singular_en.size()-2) == "fe") {
                translation = singular_en.substr(0, singular_en.size()-2) + "ves";
            } else if (singular_en.back() == 'f') {
                translation = singular_en.substr(0, singular_en.size()-1) + "ves";
            } else {
                translation = singular_en + "s";
            }
            word_type = 0;
               uint8_t f = lookupFlags(nouns, singular_pt.c_str());
                if (f & (NO_PLURAL | UNCOUNTABLE)) { // we now have bit flags, one of them check for words with no plural forms like (all).
                    translation = lookup(nouns, singular_pt.c_str()); // there's also the irregular_plural flag but who knows when i'll fkn do that
                } else if(f & IRREGULAR_PLURAL){
                    translation = lookup(nouns, singular_pt.c_str());
                     if (translation.size() >= 4 && translation[1] == 'o' && translation[2] == 'o') {
                      translation[1] = 'e';
                      translation[2] = 'e';
                    } else if(translation.size() >= 2 && translation.substr(translation.length() - 2) == "an"){
                         translation = translation.substr(0, translation.length() - 2) + "en";
                    }

                }
            translation = normalize(translation);
        }
    }
}

    // same as above for adjectives. e.g bonito[s] -> bonito, except we dont plug in 's' cause english has no adj. plurals ;p
      else if(adj_plural){

        if(lookup(adj, (word.substr(0, word.length() - 1).c_str()))){
         translation = lookup(adj, (word.substr(0, word.length() - 1).c_str()));

        }
        
        else if(lookup(adj,(word.substr(0, word.length() - 2) + "o").c_str())){
             translation = lookup(adj,(word.substr(0, word.length() - 2) + "o").c_str());
          }

      word_type = 1;
    }  
    // by switching the last letter of the word, we can check for **BASIC** gender shift. e.g cachorra -> (cachorra - a) + o -> cachorro 
   else if(gender_shift){
          
         translation = lookup(nouns, (word.substr(0, word.length() - 1) + "o").c_str());
         word_type = 0;
        }
        
    // same as above for adjectives. e.g pequena -> (pequena - a) + o -> pequeno
    else if(adj_gender_shift){

         translation = lookup(adj,(word.substr(0, word.length() - 1) + "o").c_str());
         word_type = 1;
        }
         else if(diminutive){
            if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "o").c_str())){
                translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4) + "o").c_str()));

            }else if(lookup(nouns, (word.substr(0, word.length() - 6) + "o").c_str()) ){
                translation = "little " + string(lookup(nouns, (word.substr(0, word.length() - 6) + "o").c_str()));
            }
            else  if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "a").c_str())){
                translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4) + "a").c_str()));

            }else  if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 5)).c_str())){
                translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 5).c_str())));

            }
                word_type = 0;
            }

         else if(createNounFromVerb(word).translation.size() > 0){
        translation = createNounFromVerb(word).translation;
        word_type = createNounFromVerb(word).type;
       }
         // if not found morpheme
        else if(morphemeLookup(word).translation.length() > 0){
        // if suffix not found, look for morpheme breakdown
        translation = morphemeLookup(word).translation;
        word_type = morphemeLookup(word).type;
        
      }
   
        // if preffix not found, look for prefix
       else if(prefixLookup(word).translation.length() > 0){
        translation = prefixLookup(word).translation;
        word_type = prefixLookup(word).type;
       }
       // if not found suffix match
        else if(suffixLookup(word).translation.length() > 0 || suffixLookup(word.substr(0, word.length() - 1)).translation.length() > 0){     
           string look = suffixLookup(word).translation.length() > 0 ? suffixLookup(word).translation : suffixLookup(word.substr(0, word.length() - 1)).translation + "s";
        translation = look;
        word_type = suffixLookup(word).translation.length() > 0 ? suffixLookup(word).type : suffixLookup(word.substr(0, word.length() - 1)).type;
       }
     
       else if(adjectification(word).translation.size() > 0){
        translation = adjectification(word).translation;
        word_type = adjectification(word).type;
       }
       else{
           return {word, word, -1};
           
      }

  return {word, normalize(translation), word_type};
}



// when words need to switch order
// this is actually various manipulations (Take Off The Blindfold REFERENCE????)
//not only word order, but i'm not changing the name at this point
static vector<Word> reorder_helpers(vector<Word> sentence_arr) {
    
    vector<Word> reordered_arr;
    
for (size_t j = 0; j < sentence_arr.size(); ++j) {
}

    for (size_t i = 0; i < sentence_arr.size(); ++i) {

        // ------------------------ PRONOUN ASSIGNING  -----------------
        // english verbs do not conjugate person aside from third vs non-third (and even thats an understatement
        // cause 'they' (3rd plural) falls with 1st. e.g she loves, he loves, you love, i love, we love, they love 
        // point being a portuguese verb can infer more info on the person: eu comO, você comE, eles comEM, nos comEMOS than an english one.

        // a single verb can define person: vejo -> I see
    if (sentence_arr.size() == 1 && (sentence_arr[0].type == 3 || sentence_arr[0].type == 36 )) {
    string pronoun;

    if(sentence_arr[0].word.back() == 'o' && sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) != "ndo"){
        pronoun = "I ";   
    } else if(sentence_arr[0].word.back() == 's'){
        pronoun = "we ";
    } else if(sentence_arr[0].word.back() == 'm'){
        pronoun = "they ";
    } else if(sentence_arr[0].word.back() == 'e' || sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) == "ndo"){
        pronoun = "";
    }else {
    const std::string &word = sentence_arr[0].translation;
    size_t space_pos = word.find(' ');

    if (space_pos != std::string::npos) {
        if (space_pos >= 2 && word.substr(space_pos - 2, 2) != "ed") {
            pronoun = "to ";
        }
    } else {
        if (word.size() >= 2 && word.substr(word.size() - 2, 2) != "ed") {
            pronoun = "to ";
        }
    }
}

    reordered_arr.clear(); 
    reordered_arr.push_back(Word{
        sentence_arr[0].word,
        pronoun + sentence_arr[0].translation + (sentence_arr[0].type == 36 ? " it" : ""),
        sentence_arr[0].type
    });

    return reordered_arr;
}
// demais
 if (i > 1 && sentence_arr.at(i - 1).type == 1 && sentence_arr.at(i).word == "demais") {

        reordered_arr.pop_back();  

    reordered_arr.push_back(Word{"demais", "too", 4});
    reordered_arr.push_back(Word{sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type}); 
}
    
        // ------------------------ ADJ + VERBS ----------------- 
        // yk like, díficil de jogar -> very hard of to play . you cant have an adjective and a verb with a broken connector (8)
            else if (i > 1 && sentence_arr.at(i - 2).type == 1 && sentence_arr.at(i - 1).type == 8  &&  (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
            if (sentence_arr.at(i - 1).translation == "don't" || sentence_arr.at(i - 1).translation == "doesn't") {
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
                continue;
            }
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i).type});
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"de", "to", -1}); 
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
          
    
    } 
         else if (i > 0 && sentence_arr.at(i - 1).translation == "mine" && sentence_arr.at(i - 0).type == 0 ) {
          
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"meu", "my", 40});
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
    
    } 
 else if (i > 1 && sentence_arr.at(i - 2).type == 9  && sentence_arr.at(i - 1).type == 1 && isPunctuation(sentence_arr.at(i - 0).word)) {
          
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr.at(i - 1));
            reordered_arr.push_back(Word{"one", "one", 0});
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
     break;
    } 
    //what are the pronouns called? the ones that are like, themself, itslef, myself.
    // lmk if anybody knows that, like..................
   else if (i > 1 && (sentence_arr.at(i - 2).type == 3 || sentence_arr.at(i - 2).type == 36) && sentence_arr.at(i - 1).translation == "-"  && sentence_arr.at(i).word == "se") {
    // ame-se        
    
            reordered_arr.pop_back(); 
            reordered_arr.pop_back(); 
            // verb
            reordered_arr.push_back(Word{ sentence_arr[i- 2].word, sentence_arr.at(i - 2).translation, sentence_arr.at(i).type}); 
            // yourself
            reordered_arr.push_back(Word{ "se", "yourself", sentence_arr.at(i).type}); 
        }
      // se mate  , se joga, 
           else if (i > 0 && sentence_arr.at(i - 1).word == "se" && (sentence_arr.at(i).type == 36 ||sentence_arr.at(i).type == 3)) {
   
            reordered_arr.pop_back(); 
            // verb
            reordered_arr.push_back(Word{ sentence_arr[i].word, sentence_arr.at(i).translation, sentence_arr.at(i).type}); 
            // yourself
            reordered_arr.push_back(Word{ "se", "yourself", sentence_arr.at(i).type}); 
        }

         // ------------------------ FOR/WITHOUT + VERB? IS THAT A NAME FOR THAT  --------------------
        // the construction for + verb[3 || 36], only shows up as the present continuous:
        // (for sending* me and invitation, for selling* yourself pardons)
        // (what is this food for? it's for selling*)
    else if (i > 0 && (sentence_arr.at(i - 1).translation == "for" || sentence_arr.at(i - 1).translation == "without") &&
    (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {

      reordered_arr.pop_back();   
    string fixed_verb = sentence_arr.at(i).translation.back() == 'e'
        ? sentence_arr.at(i).translation.substr(0, sentence_arr.at(i).translation.length() - 1) + "ing"
        : sentence_arr.at(i).translation + "ing";

    reordered_arr.push_back(Word{sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation,-1}); // "for"
    reordered_arr.push_back(Word{ sentence_arr.at(i).word, fixed_verb,sentence_arr.at(i).type});
    sentence_arr.at(i).type = -1;
    continue;
}

else if (i > 0 && (sentence_arr.at(i - 1).type == 8 || sentence_arr.at(i - 1).type == 13  || isPunctuation(sentence_arr.at(i - 1).word)) && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36 )) {

    string pronoun;
if(sentence_arr.at(i).word.back() == 'o' &&
   sentence_arr[0].word.length() >= 3 &&
   sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) != "ndo") {
    pronoun = "I ";
} else if(sentence_arr.at(i).word.back() == 's'){
    pronoun = "we ";
} else if(sentence_arr.at(i).word.back() == 'e' || 
          (sentence_arr[0].word.length() >= 3 && sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) == "ndo")) {
    pronoun = ""; 
} else {
    pronoun = ""; 
}

    if (!reordered_arr.empty() && reordered_arr.back().translation == sentence_arr.at(i - 1).translation) {
        reordered_arr.pop_back();
    }
    
    reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});
    reordered_arr.push_back(Word{sentence_arr.at(i).word, pronoun + sentence_arr.at(i).translation,sentence_arr.at(i).type});

}
             else if (i == 0 && (sentence_arr[0].type == 3 || sentence_arr[0].type == 36)) {
    
    string pronoun;
    string pronoun_tr;
    if (sentence_arr[0].word.back() == 'o' && sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) != "ndo"){ 
        pronoun = "I"; pronoun_tr = "eu";
    } 
    else if (sentence_arr[0].word.back() == 's'){ 
        pronoun = "We"; pronoun_tr = "nós";
    }
     else if(sentence_arr[0].word.back() == 'e' || sentence_arr[0].word.substr(sentence_arr[0].word.length() - 3) == "ndo"){
        pronoun = ""; 
    }
    else {
        pronoun = "to";
         pronoun_tr = "$inf";
    };

    reordered_arr.push_back(Word{pronoun_tr, pronoun, 4});  // Add pronoun as type 4
    reordered_arr.push_back(Word{sentence_arr[0].word, sentence_arr[0].translation ,sentence_arr[0].type});  
 
}


       // -------------------- KIND OF MEIO [ADJ] -----------------------------------------
else if (i > 0 && sentence_arr.at(i - 1).translation == "middle" && sentence_arr.at(i).type == 1) {
    if (!reordered_arr.empty())
        reordered_arr.pop_back();  

    reordered_arr.push_back(Word{"meio", "kind of", -1});
    reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
    continue;  
}

  // ------------------------ ADJECTIVE ORDER  -----------------
        // a set is noun[0] and adjective[1], we switch order, so that casa[0] azul[1] -> blue[1] house[0]
        else if (i > 0 && sentence_arr.at(i - 1).type == 0 && sentence_arr.at(i).type == 1) {
            reordered_arr.pop_back();   

            reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, 1});  
            reordered_arr.push_back(Word{sentence_arr[i- 1].word, sentence_arr.at(i - 1).translation, 0}); 
            
        }

      

       
  // ------------------------ ARTICLE VS PREPOSITION (e.g A VS À) COMPETITION  --------------------
        // when a match is 'a', is the next word a pronoun[4] or an article[9]? that means its "à" if not "a"
        // since it defaults to 'the' i dont think i need a fallback                                                            // this is problably wrong, but ideally -1 should be proper nouns SOMEHOW.
        else if (i > 0 && sentence_arr.at(i - 1).translation == "the" && (sentence_arr.at(i).type == 4 || sentence_arr.at(i).type == 9 || sentence_arr.at(i).type == -1)) {
          reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"à","to", 20});
            reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});  
        } 
        // for verbs (e.g ajudar a* desenvolver)
                else if (i > 1 && (sentence_arr.at(i - 2).type == 3 || sentence_arr.at(i - 2).type == 36) && sentence_arr.at(i - 1).translation == "the" && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
          reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"à","to", 20});
            reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});  
        } 
          
        // ------------------------ NOT VS DON'T ----------------- TODO: i'm sure theres more cases where not is dont, and vice versa, ALSO THERES NO. 
        // FUCLKKKKKKKKKK THERES ALSO "DOESN'T" FOR THIRD PERSON
        // a set is pronoun[4] + "no"  + verb[3]. 'no' becomes then 'don't' or doesn't so that não* gosto[3] -> don't like instead of 'no like'.
    // Handle: "not" + modal verb + following word (fixing "not can today" -> "can not today")
else if (i > 1 && sentence_arr.at(i - 1).translation == "not") {
    // Subject
    string subj = (i >= 2) ? sentence_arr.at(i - 2).word : ""; 
    string subj_trans = (i >= 2) ? sentence_arr.at(i - 2).translation : "";

    string verb = sentence_arr.at(i).translation;
    string verb_word = sentence_arr.at(i).word;
    int verb_type = sentence_arr.at(i).type;

    while (!reordered_arr.empty() &&
           (reordered_arr.back().translation == "not" ||
            reordered_arr.back().translation == verb ||
            reordered_arr.back().word == subj)) {
        reordered_arr.pop_back();
    }

    if (std::find(modals.begin(), modals.end(), verb) != modals.end()) {
        reordered_arr.push_back(Word{subj, subj_trans, 4});
        reordered_arr.push_back(Word{verb_word, verb + " not", 3});
        i++;
    } 
    else if (!subj.empty()) {
        string aux = (std::find(th_per_aux.begin(), th_per_aux.end(), subj_trans) != th_per_aux.end())
                        ? "doesn't" : "don't";

        reordered_arr.push_back(Word{subj, subj_trans, 4});       // subject
        reordered_arr.push_back(Word{"não", aux, 3});             // aux + not
        reordered_arr.push_back(Word{verb_word, verb, verb_type}); // verb

        i++;
    }

    // Now push **all remaining words after the verb** (like objects, complements)
    while (i < sentence_arr.size() && sentence_arr.at(i).translation != "not") {
        reordered_arr.push_back(sentence_arr.at(i));
        i++;
    }

    continue;
}

                // ------------------------ DOUBLE VERBS ----------------- 
            // a set is verb[3] and verb[3], we add 'to' between them, so that amo[3] correr[3] -> love[3] *to* run[3]
            else if (i > 0 && (sentence_arr.at(i - 1).type == 3 || sentence_arr.at(i - 1).type == 36) && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
                if (sentence_arr.at(i - 1).translation == "don't" || sentence_arr.at(i - 1).translation == "doesn't") {
                    reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
                    continue;
                }
                reordered_arr.pop_back(); 
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});  
                if (std::find(modals.begin(), modals.end(), sentence_arr.at(i - 1).translation) == modals.end()) {
                    
                // reordered_arr.push_back(Word{"to", -1});  
                // i believe +ing is better than verb [to] verb here. 'i love running' vs 'i love to run'
                // we also need consonant duplication, runing => running
               reordered_arr.push_back(Word{"to", "to", -1});
                reordered_arr.push_back( Word{sentence_arr.at(i).word,
                    sentence_arr.at(i).translation,    
                    sentence_arr.at(i).type
                });
                
                }else{
                     reordered_arr.push_back( Word{sentence_arr.at(i).word,
                    sentence_arr.at(i).translation,    
                    sentence_arr.at(i).type
                });
                }
            }
   
          // ------------------------ ADJ + VERBS ----------------- 
        // yk like, muito fácil correr -> very easy run. you cant have an adjective and a verb without the connector "to"
        else if (i > 1 &&sentence_arr.at(i - 1).type == 1  &&  (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
                if (i > 1 && sentence_arr.at(i - 2).type == 0 && sentence_arr.at(i - 1).type == 1) {
        // This is actually part of a noun-adjective-verb sequence, skip this rule
        reordered_arr.push_back(sentence_arr.at(i));
        continue;
    }
            if (sentence_arr.at(i - 1).translation == "don't" || sentence_arr.at(i - 1).translation == "doesn't") {
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
                continue;
            }
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i).type});
            reordered_arr.push_back(Word{"de", "to", -1}); 
            reordered_arr.push_back(Word{  sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
        }

        
     // ------------------------ OBLIQUE PRONOUNS  -----------------
        // a set is oblique pronoun[11] and verb[3], we switch order, so that te[11] amo[3] -> love[3] you[11]
        else if (i > 0 && sentence_arr.at(i - 1).type == 11 && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
        
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});  
            reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});
        } 

    // intransitive verbs, just plug an "it" at the end, theres way more complicated nuance 
    // but i'm not doing allat now.
    // so if 'eu[2] amo[36]' => 'i[2] (love it[36])]!  
       else  if (sentence_arr.at(i).type == 36) {
        
                bool add_it = true;

                if (i + 1 < sentence_arr.size()) {
                    int next_type = sentence_arr[i + 1].type;
                    if (next_type == 3 || next_type == 36 || next_type == 0 || next_type == 1 || next_type == 4 || next_type == -1 ||  next_type == 2 || next_type == 9  || next_type == 8 || next_type == 13 || next_type == 40) {
                        add_it = false; 
                    }
                }
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});

                if (add_it && std::find(modals.begin(), modals.end(), sentence_arr.at(i).translation) == modals.end()) {
                    reordered_arr.push_back(Word{"$it", "it", -1});
                    
                }
            continue;
            }

       
         
        // ------------------------ SUPERLATIVE  --------------------
        // a set is word1 = 'o/a' and word2 = 'mais' and word3 = adj[1], we eliminate the 'mais', so that 'o mais forte[2]' -> 'the strongest'
        else if (i > 1 && sentence_arr.at(i - 2).type == 9  && sentence_arr.at(i - 1).translation == "more" && sentence_arr.at(i).type == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"o mais " + sentence_arr.at(i).word, sentence_arr.at(i).translation + "est", 20});  
        } 

        // ------------------------ COMPARATIVE  -----------------
        // a set is word1 = 'mais' and word2 = adj[1], we eliminate the first 'mais' and append 'er' to word2, so that mais forte[2] -> stronger
        else if (i > 0 && sentence_arr.at(i - 1).translation == "more" && sentence_arr.at(i).type == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ "mais " + sentence_arr.at(i).word, sentence_arr.at(i).translation + (sentence_arr.at(i).translation.back() == 'e' ? "r" : "er"), 20});  
        } 

        // ------------------------ INTRANSITIVE VERBS THAT TAKE 'DE'  ----------------- 
        // a set is word1 = verb[3] and word2 = 'de', we eliminate the preposition 'de', so that gosto[1] de  -> stronger
        else if (i > 0 && (sentence_arr.at(i - 1).type == 3 || sentence_arr.at(i - 1).type == 36) && sentence_arr.at(i).translation == "of") {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});  
        } 
          // ------------------------ DOUBLE NOUNS ----------------- TODO: nuance? 
        // a set is noun[0] and "de" and noun[0], we invert them and remove the 'de/of' between them, so that "suco[0] de* laranja[0]" -> orange[0] juice[0]
       else if (i > 1 && sentence_arr.at(i - 2).type == 0  && sentence_arr.at(i - 1).translation == "of" && sentence_arr.at(i).type == 0) {
    reordered_arr.pop_back();
    reordered_arr.pop_back();   
    reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});  
    reordered_arr.push_back(Word{ sentence_arr.at(i - 2).word, sentence_arr.at(i - 2).translation, sentence_arr.at(i - 2).type});
    continue;  // IMPORTANT: skip other rules
}


        // ------------------------ TRANSITIVE VERBS WITH A PERSONAL PRONOUN  ----------------- 
        // a set is word1 = verb[3] and word2 = pronoun[4], we use the second value of the pair? of the pronoun i guess idk i'm tired
        // like the value of the key 'ela' is a pair<string, string> that holds both the subject and object pronoun {'she', 'her'}
        else if (i > 0 && (sentence_arr.at(i - 1).type == 3 || sentence_arr.at(i - 1).type == 36) && sentence_arr.at(i).type == 4) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr.at(i - 1).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});
            reordered_arr.push_back(Word{ sentence_arr.at(i).word,
              lookup(obj_pro, sentence_arr.at(i).translation.c_str()) 
                  ? string(lookup(obj_pro, sentence_arr.at(i).translation.c_str())) 
                  : sentence_arr.at(i).translation,
              10
          });
        }

        // ------------------------ CONTINUOUS TO BE (IS, ARE, AM)  ----------------- 
        // a set is word1 = pronoun[4] and word2 = "is", use an if to check the first pronoun and change it accordingly 
        // she is, i am, we are
        else if (i > 0 && sentence_arr.at(i - 1).type == 4 && sentence_arr.at(i).translation == "is") {
            string corr_pro = "is";
            reordered_arr.pop_back(); 
            if(sentence_arr.at(i - 1).translation == "she" || sentence_arr.at(i - 1).translation == "he"){
                corr_pro = "is";
            } else if(sentence_arr.at(i - 1).translation == "we" || sentence_arr.at(i - 1).translation == "they" || sentence_arr.at(i - 1).translation == "you"){
                corr_pro = "are";
            } else if(sentence_arr.at(i - 1).translation == "i"){
                corr_pro = "am";
            };
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i - 1).translation, sentence_arr.at(i - 1).type});   
            reordered_arr.push_back(Word{"$ser", corr_pro, 4});
        } 


        // same thing but when the verbs are connected by 'que' (e.g: tenho QUE ver)
        else if (i > 1 && (sentence_arr.at(i - 2).type == 3 || sentence_arr.at(i - 2).type == 36) && sentence_arr.at(i - 1).translation == "what"  && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"que", "to", -1});  
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type}); 
        }

        else if (i > 1 && (sentence_arr.at(i - 2).type == 3 || sentence_arr.at(i - 2).type == 3) && sentence_arr.at(i - 1).translation == "of" && (sentence_arr.at(i).type == 3 || sentence_arr.at(i).type == 36)) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{sentence_arr.at(i - 2).word, sentence_arr.at(i - 2).translation, sentence_arr.at(i - 2).type});  
            reordered_arr.push_back(Word{"de", "to", -1});  
            reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
        } 

      

        else {
            if (sentence_arr.at(i).type != -1)  // only push if not processed
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
        }
    }

    // new layer. LIKE THE CONE LAYER, FROM THE MOVIE
    // this guy will do word manipulation, the above one will take the name literally (reorder helpers)
    // needed to do this cause handling it all on the same iteration on the original array was impossible    

     for (size_t i = 0; i < reordered_arr.size(); ++i) {
    if (reordered_arr[i].translation == "is") {
        
        bool preceded_by_pronoun = (i > 0 &&
            (reordered_arr[i-1].type == 4 || reordered_arr[i-1].type == 0 || reordered_arr[i-1].type == 13 || reordered_arr[i-1].type == 2 || reordered_arr[i-1].type == 3 || reordered_arr[i-1].type == 36));

        if (preceded_by_pronoun) {
            reordered_arr[i].translation = "is";
        } else {
            
            reordered_arr[i].translation = "it is";
            i++; 
        }
    }    // ------------------------ ARTICLE TWEAKS  --------------------
        // does the next translation start in a vowel? if so the article[9] should be an. a apple -> an apple
       else if (i > 0 && reordered_arr[i - 1].type == 9 && isVowel(reordered_arr[i].translation[0]) && (reordered_arr[i].type == 0 || reordered_arr[i].type == 1)) {
            string article = reordered_arr[i - 1].translation;
            if (article != "the") article += "n"; 
            reordered_arr[i - 1].translation = article; 

}


//VAI TOMAR NO CU ESSA PORRA VSFFFFFFFFFFFFFFFFFFFFFFFF QUE ODIO 
else if (sentence_arr.size() >= 2 && i == sentence_arr.size() - 1 && sentence_arr.at(i - 1).type == 9 && sentence_arr.at(i).type == 1) { 
       bool add_one = true;

        if (i + 1 < sentence_arr.size()) {
            int next_type = sentence_arr[i + 1].type;
            if (next_type == 0 || next_type == 3 || next_type == 10) {
                add_one = false;
            }
            if(isPunctuation(sentence_arr[i + 1].translation)){
                add_one = true;
            }
        }

        if (add_one) {
            reordered_arr.pop_back();
             reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
            reordered_arr.push_back(Word{"one", "one", 0});
            if (i + 1 < sentence_arr.size()) reordered_arr.push_back(Word{sentence_arr[i + 1].word, sentence_arr[i + 1].translation, sentence_arr[i + 1].type});
        }
        continue;
    }

   else if (sentence_arr.size() >= 3 && i == sentence_arr.size() - 1 && sentence_arr.at(i - 2).type ==  9 && sentence_arr.at(i - 1).type == 1 && isPunctuation(sentence_arr.at(i).translation)) {     bool add_one = true;

        if (i + 1 < sentence_arr.size()) {
            int next_type = sentence_arr[i + 1].type;
            if (next_type == 0 || next_type == 3 || next_type == 10) {
                add_one = false;
            }
            if(isPunctuation(sentence_arr[i + 1].translation)){
                add_one = true;
            }
        }

        if (add_one) {
            reordered_arr.pop_back(); reordered_arr.pop_back();
            
             reordered_arr.push_back(Word{sentence_arr.at(i - 1).word, sentence_arr[i- 1].translation, sentence_arr.at(i - 1).type});
            reordered_arr.push_back(Word{"one", "one", 0});
            
             reordered_arr.push_back(Word{sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
            }
        continue;
    }


}





if (!sentence_arr.empty()) {

    // this is preposition in, on, at disambiguation;
    // every noun has a two flags ON and IS_PLACE
   for (size_t i = 0; i < reordered_arr.size(); ++i) {
    auto &current = reordered_arr.at(i);
    // if no or na "in the";
    if (current.word == "no" || current.word == "na") {
        if (i + 1 < reordered_arr.size()) {
            const auto &next = reordered_arr.at(i + 1);
            if (next.type == 0) {
                //get the flags
                uint8_t f = lookupFlags(nouns, next.word.c_str());
                if (f & ON) current.translation = "on the"; // if flag ON  is raised no(a) = "on the"
                else if (f & IS_PLACE) current.translation = "at the";// if flag IS_PLACE is raised no(a) = "at the"
                else current.translation = "in the"; // if none of the above, default to in
            }
        }
    }
}

if(reordered_arr.size() > 2 && reordered_arr[0].word == "$inf" && reordered_arr[2].translation == "a"){
   if(reordered_arr[reordered_arr.size() - 1].word == "?"){
       reordered_arr[0].translation = "is there"; 
       
    }else{
        reordered_arr[0].translation = "there is"; 
       
    }
     reordered_arr[1].translation = ""; 
}

if(reordered_arr.size() > 2 && reordered_arr[0].word == "$inf" && reordered_arr[2].word.back() == 's'){
 reordered_arr[0].translation = "there are"; 
  reordered_arr[1].translation = ""; 
}





// interrogative

    string last = sentence_arr[sentence_arr.size() - 1].word;
    
    //　QUESTION　QUESTION　僕は　QUESTION　QUESTION　いったい　QUESTION　QUESTION　君の何を知っていたの????????????
    if(last == "?"  && sentence_arr[0].type != 13 && sentence_arr[0].translation != "what is"){
        
        reordered_arr.clear();  // clear previously added elements

        // look for a pronoun
        // nouns should do this as well, and always have the pronoun being "does"
        string pronoun_ = "";
        int pronoun_index = -1;
        int word_type;
        bool isModal = false;
        for (size_t i = 0; i < sentence_arr.size(); ++i) {
            if (sentence_arr.at(i).type == 4 || sentence_arr.at(i).type == 0) {
                pronoun_ = sentence_arr.at(i).translation; 
                pronoun_index = i;
                word_type = sentence_arr.at(i).type;
                break;                
            }
            
        }
        if (!pronoun_.empty() && pronoun_index >= 0) {
            // is this pronoun in the third person vector?
            string aux = (std::find(th_per_aux.begin(), th_per_aux.end(), pronoun_) != th_per_aux.end()) ? "does" : "do";
         if (pronoun_index + 1 < sentence_arr.size() && 
         std::find(modals.begin(), modals.end(), sentence_arr[pronoun_index + 1].translation) != modals.end())   {
                aux = sentence_arr[pronoun_index + 1].translation;
                isModal = true;
            }
            if(word_type == 0) aux = "does";

            // push everything before the pronoun as is
            for(int i = 0; i < pronoun_index; ++i){
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
            }

            // push pronoun + verb together
            string question_phrase = aux + " " + sentence_arr[pronoun_index].translation;
            if (pronoun_index + 1 < sentence_arr.size() - 1) {
                if ((sentence_arr[pronoun_index + 1].type == 3 || sentence_arr[pronoun_index + 1].type == 36) && sentence_arr[pronoun_index + 1].translation.back() != 's') {
                    question_phrase +=  (!isModal ? sentence_arr[pronoun_index + 1].translation : "");
                }else{
                      question_phrase += (!isModal ? sentence_arr[pronoun_index + 1].translation.substr(0, sentence_arr[pronoun_index + 1].translation.length() - 1) : "");
                }
            }
            reordered_arr.push_back(Word{ sentence_arr[pronoun_index].word, question_phrase, sentence_arr[pronoun_index].type});

            // push remaining words (like "?") after that
            for(int i = pronoun_index + 2; i < sentence_arr.size(); ++i){
                reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});
            }
        }
    }
}


// tribunal de minusculas causas (resolve duplication, double spacing, and other stuff that disrtupts the peace)
reordered_arr.erase(
    std::remove_if(reordered_arr.begin(), reordered_arr.end(),
        [](const Word &w) {
            return w.translation.empty() ||
                   std::all_of(w.translation.begin(), w.translation.end(), ::isspace);
        }),
    reordered_arr.end()
);

//statistics layer
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
//homonym mediation
for (size_t i = 0; i < final_arr.size(); ++i) {
    if (final_arr[i].word == "manga" || 
        final_arr[i].word == "banco" || 
        final_arr[i].word == "pena" || 
        final_arr[i].word == "novo" || final_arr[i].word == "nova" ||
        final_arr[i].word == "alto" || final_arr[i].word == "alta" ||
        final_arr[i].word == "rosa"
    
    ) {
        
        int start = max(0, static_cast<int>(i) - 2);
        int end   = min(static_cast<int>(final_arr.size()) - 1, static_cast<int>(i) + 2);

        vector<string> context;
        for (int j = start; j <= end; ++j) {
            context.push_back(final_arr[j].translation);
        }

        // But pass the PORTUGUESE word to semantics for homonym lookup
        size_t contextIndex = static_cast<size_t>(i - start);
        
        // Create a temporary context with the Portuguese word at the target position
        vector<string> portuguese_context = context;
        portuguese_context[contextIndex] = final_arr[i].word;  // Use Portuguese word for lookup
        
        string resolved_word = semantics(portuguese_context, contextIndex, homonyms, homonymCount);

        final_arr[i].translation = resolved_word;
    }
}


    return final_arr;
}

//ngram groups
static std::string unigramLookup(vector<string> array_of_words, vector<int> ignore_flags){

  vector<Word> sentence_arr;
  vector<Word> word_arr;

  int match_type;
  string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    
    Word match = nounLookup(array_of_words[i]);
    switch (ignore_flags[i])
    {
    case 0:{
    match_type = match.type;
    if(match.type == -1) match_type = 0;
    
         Word match_ = {array_of_words[i], match.translation, match_type};
        sentence_arr.push_back({match.word, match.translation ,match_type});
        word_arr.push_back(match_);
        break;}
    case 1:{
        Word match_ = {array_of_words[i], array_of_words[i], 0};
       sentence_arr.push_back({array_of_words[i], array_of_words[i],0});
         word_arr.push_back(match_);
       break;}
    default:
      break;
    }
  }
  if(word_arr.size() > 0) sentence_arr = reorder_helpers(word_arr);

  
 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const std::string& token = sentence_arr.at(i).translation;

    char firstChar = token.empty() ? '\0' : token[0];
    bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                          firstChar == '.' || firstChar == ','
                          || firstChar == '-' || firstChar == '/' || firstChar == ':');

    if (!sentence.empty() && !isPunctuation) {
        sentence += " ";
    }

    sentence += token;
}
  return sentence;
}

static std::string bigramLookup(const std::vector<std::string>& words, std::vector<int>& ignore_flags) {
    std::vector<std::string> mended_array_of_words;
    std::vector<int> new_ignore_flags;

    size_t i = 0;
    while (i < words.size()) {
        if (i + 1 < words.size() && ignore_flags[i] == 0 && ignore_flags[i + 1] == 0) {
            std::string bigram = words[i] + "_" + words[i + 1];
            const char* bigram_translation = lookup(fixed_ngrams, bigram.c_str());
            
            if (bigram_translation) {
                mended_array_of_words.push_back(bigram_translation);
                new_ignore_flags.push_back(1);  
                i += 2;  
                continue;
            }
        }
        
        mended_array_of_words.push_back(words[i]);
        new_ignore_flags.push_back(ignore_flags[i]);
        i++;
    }

    return unigramLookup(mended_array_of_words, new_ignore_flags);
}

static std::string trigramLookup(const std::vector<std::string>& words) {
    std::vector<std::string> mended;
    std::vector<int> ignore_flags(words.size(), 0);

    size_t i = 0;
    while (i < words.size()) {
        if (i + 2 < words.size()) {
            std::string trigram = words[i] + "_" + words[i + 1] + "_" + words[i + 2];
            const char* trigram_translation = lookup(fixed_ngrams, trigram.c_str());
            
            if (trigram_translation) {
                mended.push_back(trigram_translation);
                ignore_flags.push_back(1);  
                i += 3;  
                continue;
            }
        }
        mended.push_back(words[i]);
        ignore_flags.push_back(0);
        i++;
    }

    // Then process bigrams on the result
    return bigramLookup(mended, ignore_flags);
}

std::string traduzir_en(const char* sentence) {
    char buffer[250];
    strncpy(buffer, sentence, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    to_lower(buffer);
    vector<string> arr = tokenize(string(buffer));  
    std::string translated = trigramLookup(arr);
    
    return script_adequation(translated); 
}

#endif